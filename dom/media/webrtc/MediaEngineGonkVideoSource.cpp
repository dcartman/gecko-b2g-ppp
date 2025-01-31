/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */
#include "MediaEngineGonkVideoSource.h"

#include "CameraControlListener.h"
#include "gfxPlatform.h"
#include "GrallocImages.h"
#include "libyuv.h"
#include "mozilla/dom/MediaTrackSettingsBinding.h"
#include "mozilla/ErrorNames.h"
#include "mozilla/layers/GrallocTextureClient.h"
#include "mozilla/layers/ImageBridgeChild.h"
#include "mozilla/ReentrantMonitor.h"
#include "mozilla/widget/ScreenManager.h"
#include "ScreenOrientation.h"

namespace mozilla {

using android::GraphicBuffer;
using android::sp;
using dom::MediaTrackConstraints;
using gfx::IntSize;
using mozilla::layers::ImageContainer;

extern LazyLogModule gMediaManagerLog;
#define LOG(...) MOZ_LOG(gMediaManagerLog, LogLevel::Debug, (__VA_ARGS__))

class CameraControlWrapper : public CameraControlListener {
 public:
  NS_IMETHOD_(MozExternalRefCountType) AddRef(void);
  NS_IMETHOD_(MozExternalRefCountType) Release(void);

  explicit CameraControlWrapper(const nsACString& aCameraName);

  void SetPhotoOrientation(int aOrientation);
  bool GetIsBackCamera() { return mIsBackCamera; }
  int GetCameraAngle() { return mCameraAngle; }

  nsresult Allocate(MediaEngineGonkVideoSource* aOwner);
  nsresult Deallocate();
  nsresult Start(webrtc::CaptureCapability aCapability);
  nsresult Stop();
  nsresult TakePhoto(MediaEnginePhotoCallback* aCallback);

  // These methods are CameraControlListener callbacks.
  void OnHardwareStateChange(HardwareState aState, nsresult aReason) override;
  bool OnNewPreviewFrame(layers::Image* aImage, uint32_t aWidth,
                         uint32_t aHeight) override;
  void OnTakePictureComplete(const uint8_t* aData, uint32_t aLength,
                             const nsAString& aMimeType) override;
  void OnUserError(UserContext aContext, nsresult aError) override;

 private:
  ~CameraControlWrapper() {
    Stop();
    Deallocate();
  }

  int mCaptureIndex = -1;
  nsAutoCString mCameraName;
  bool mIsBackCamera = false;
  int mCameraAngle = 0;
  int mOrientation = 0;
  bool mOrientationChanged = true;

  HardwareState mHardwareState = kHardwareUninitialized;
  RefPtr<ICameraControl> mCameraControl;
  nsTArray<RefPtr<MediaEnginePhotoCallback>> mPhotoCallbacks;

  // Monitor for camera callback handling.
  mozilla::ReentrantMonitor mMonitor;

  // Keep a weak reference of MediaEngineGonkVideoSource.
  MediaEngineGonkVideoSource* mOwner = nullptr;
};

NS_IMPL_ADDREF_INHERITED(CameraControlWrapper, CameraControlListener)
NS_IMPL_RELEASE_INHERITED(CameraControlWrapper, CameraControlListener)

CameraControlWrapper::CameraControlWrapper(const nsACString& aCameraName)
    : mCameraName(aCameraName),
      mIsBackCamera(aCameraName.EqualsASCII("back")),
      mMonitor("CameraControlWrapper::mMonitor") {
  // Find the index of the specified camera name.
  int num = 0;
  ICameraControl::GetNumberOfCameras(num);
  for (int i = 0; i < num; i++) {
    nsAutoCString name;
    ICameraControl::GetCameraName(i, name);
    if (name == aCameraName) {
      mCaptureIndex = i;
      break;
    }
  }
}

void CameraControlWrapper::SetPhotoOrientation(int aOrientation) {
  ReentrantMonitorAutoEnter sync(mMonitor);
  if (aOrientation != mOrientation) {
    mOrientationChanged = true;
    mOrientation = aOrientation;
  }
}

nsresult CameraControlWrapper::Allocate(MediaEngineGonkVideoSource* aOwner) {
  ReentrantMonitorAutoEnter sync(mMonitor);

  if (mCameraControl) {
    return NS_ERROR_FAILURE;
  }
  mCameraControl = ICameraControl::Create(mCaptureIndex);
  if (!mCameraControl) {
    return NS_ERROR_FAILURE;
  }

  // Add this as a listener for CameraControl events. We don't need
  // to explicitly remove this--destroying the CameraControl object
  // in Deallocate() will do that for us.
  mCameraControl->AddListener(this);
  mOwner = aOwner;
  return NS_OK;
}

nsresult CameraControlWrapper::Deallocate() {
  ReentrantMonitorAutoEnter sync(mMonitor);

  mOwner = nullptr;
  mCameraControl = nullptr;
  return NS_OK;
}

nsresult CameraControlWrapper::Start(webrtc::CaptureCapability aCapability) {
  ReentrantMonitorAutoEnter sync(mMonitor);

  MOZ_ASSERT(mCameraControl);
  if (!mCameraControl || mHardwareState == kHardwareOpen) {
    return NS_ERROR_FAILURE;
  }
  mHardwareState = kHardwareUninitialized;

  // Start camera without starting preview.
  mCameraControl->Start();

  // Wait for hardware state change.
  while (mHardwareState == kHardwareUninitialized) {
    mMonitor.Wait();
  }
  if (mHardwareState != kHardwareOpen) {
    return NS_ERROR_FAILURE;
  }

  mCameraControl->Get(CAMERA_PARAM_SENSORANGLE, mCameraAngle);
  MOZ_ASSERT(mCameraAngle == 0 || mCameraAngle == 90 || mCameraAngle == 180 ||
             mCameraAngle == 270);

  nsTArray<nsString> focusModes;
  mCameraControl->Get(CAMERA_PARAM_SUPPORTED_FOCUSMODES, focusModes);
  for (auto& focusMode : focusModes) {
    if (focusMode.EqualsASCII("continuous-video")) {
      mCameraControl->Set(CAMERA_PARAM_FOCUSMODE, focusMode);
      mCameraControl->ResumeContinuousFocus();
      break;
    }
  }

  auto width = aCapability.width;
  auto height = aCapability.height;
  if (mCameraAngle == 90 || mCameraAngle == 270) {
    std::swap(width, height);
  }

  // Start preview.
  ICameraControl::Configuration config;
  config.mMode = ICameraControl::kPictureMode;
  config.mPreviewSize.width = width;
  config.mPreviewSize.height = height;
  config.mPictureSize.width = width;
  config.mPictureSize.height = height;
  mCameraControl->Start(&config);
  return NS_OK;
}

nsresult CameraControlWrapper::Stop() {
  ReentrantMonitorAutoEnter sync(mMonitor);

  if (mHardwareState != kHardwareOpen) {
    return NS_ERROR_FAILURE;
  }
  mCameraControl->Stop();

  // Wait for hardware state change.
  while (mHardwareState == kHardwareOpen) {
    mMonitor.Wait();
  }
  return NS_OK;
}

nsresult CameraControlWrapper::TakePhoto(MediaEnginePhotoCallback* aCallback) {
  ReentrantMonitorAutoEnter sync(mMonitor);

  // If other callback exists, that means there is a captured picture on the
  // way, it doesn't need to TakePicture() again.
  if (mPhotoCallbacks.IsEmpty()) {
    if (mOrientationChanged) {
      mOrientationChanged = false;
      ICameraControlParameterSetAutoEnter batch(mCameraControl);
      // It changes the orientation value in EXIF information only.
      mCameraControl->Set(CAMERA_PARAM_PICTURE_ROTATION, mOrientation);
    }
    nsresult rv = mCameraControl->TakePicture();
    if (NS_FAILED(rv)) {
      return rv;
    }
  }
  mPhotoCallbacks.AppendElement(aCallback);
  return NS_OK;
}

void CameraControlWrapper::OnHardwareStateChange(HardwareState aState,
                                                 nsresult aReason) {
  ReentrantMonitorAutoEnter sync(mMonitor);
  mHardwareState = aState;
  mMonitor.Notify();
}

bool CameraControlWrapper::OnNewPreviewFrame(layers::Image* aImage,
                                             uint32_t aWidth,
                                             uint32_t aHeight) {
  ReentrantMonitorAutoEnter sync(mMonitor);
  if (mHardwareState != kHardwareOpen) {
    return false;
  }
  return mOwner->OnNewPreviewFrame(aImage, aWidth, aHeight);
}

void CameraControlWrapper::OnTakePictureComplete(const uint8_t* aData,
                                                 uint32_t aLength,
                                                 const nsAString& aMimeType) {
  // Create a main thread runnable to generate a blob and call all current
  // queued MediaEnginePhotoCallbacks.
  class GenerateBlobRunnable : public Runnable {
   public:
    GenerateBlobRunnable(nsTArray<RefPtr<MediaEnginePhotoCallback>>& aCallbacks,
                         const uint8_t* aData, uint32_t aLength,
                         const nsAString& aMimeType)
        : Runnable("TakePhotoError"), mPhotoDataLength(aLength) {
      mCallbacks.SwapElements(aCallbacks);
      mPhotoData = (uint8_t*)malloc(aLength);
      memcpy(mPhotoData, aData, mPhotoDataLength);
      mMimeType = aMimeType;
    }

    NS_IMETHOD Run() override {
      RefPtr<dom::Blob> blob = dom::Blob::CreateMemoryBlob(
          nullptr, mPhotoData, mPhotoDataLength, mMimeType);
      for (auto& callback : mCallbacks) {
        RefPtr<dom::Blob> tempBlob = blob;
        callback->PhotoComplete(tempBlob.forget());
      }
      // MediaEnginePhotoCallback needs to dereference on main thread.
      mCallbacks.Clear();
      return NS_OK;
    }

    nsTArray<RefPtr<MediaEnginePhotoCallback>> mCallbacks;
    uint8_t* mPhotoData;
    nsString mMimeType;
    uint32_t mPhotoDataLength;
  };

  ReentrantMonitorAutoEnter sync(mMonitor);

  // It needs to start preview because Gonk camera will stop preview while
  // taking picture.
  mCameraControl->StartPreview();

  // All elements in mPhotoCallbacks will be swapped in GenerateBlobRunnable
  // constructor. This captured image will be sent to all the queued
  // MediaEnginePhotoCallbacks in this runnable.
  if (mPhotoCallbacks.Length()) {
    NS_DispatchToMainThread(
        new GenerateBlobRunnable(mPhotoCallbacks, aData, aLength, aMimeType));
  }
}

void CameraControlWrapper::OnUserError(UserContext aContext, nsresult aError) {
  // A main thread runnable to send error code to all queued
  // MediaEnginePhotoCallbacks.
  class TakePhotoError : public Runnable {
   public:
    TakePhotoError(nsTArray<RefPtr<MediaEnginePhotoCallback>>& aCallbacks,
                   nsresult aRv)
        : Runnable("TakePhotoError"), mRv(aRv) {
      mCallbacks.SwapElements(aCallbacks);
    }

    NS_IMETHOD Run() override {
      for (auto& callback : mCallbacks) {
        callback->PhotoError(mRv);
      }
      // MediaEnginePhotoCallback needs to dereference on main thread.
      mCallbacks.Clear();
      return NS_OK;
    }

   protected:
    nsTArray<RefPtr<MediaEnginePhotoCallback>> mCallbacks;
    nsresult mRv;
  };

  ReentrantMonitorAutoEnter sync(mMonitor);

  if (aContext == UserContext::kInTakePicture && mPhotoCallbacks.Length()) {
    NS_DispatchToMainThread(new TakePhotoError(mPhotoCallbacks, aError));
  }
}

class MediaEngineGonkVideoSource::ScreenObserver final : public nsIObserver {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS

  ScreenObserver(MediaEngineGonkVideoSource* aOwner)
      : mOwner(aOwner), mOwnerThread(GetCurrentSerialEventTarget()) {}

  void Start() {
    NS_DispatchToMainThread(NS_NewRunnableFunction(
        "ScreenObserver::Start", [this, self = RefPtr<ScreenObserver>(this)]() {
          nsCOMPtr<nsIObserverService> os = services::GetObserverService();
          if (os) {
            os->AddObserver(this, "screen-information-changed", false);
          }
          UpdateOrientation();
        }));
  }

  void Stop() {
    NS_DispatchToMainThread(NS_NewRunnableFunction(
        "ScreenObserver::Stop", [this, self = RefPtr<ScreenObserver>(this)]() {
          nsCOMPtr<nsIObserverService> os = services::GetObserverService();
          if (os) {
            os->RemoveObserver(this, "screen-information-changed");
          }
        }));
  }

  void Shutdown() {
    MOZ_ASSERT(mOwnerThread->IsOnCurrentThread());
    mOwner = nullptr;
  }

  NS_IMETHOD Observe(nsISupports* aSubject, const char* aTopic,
                     const char16_t* aData) override {
    MOZ_ASSERT(NS_IsMainThread());
    if (!strcmp(aTopic, "screen-information-changed")) {
      UpdateOrientation();
    }
    return NS_OK;
  }

 private:
  ~ScreenObserver() = default;

  void UpdateOrientation() {
    MOZ_ASSERT(NS_IsMainThread());

    RefPtr<widget::Screen> screen =
        widget::ScreenManager::GetSingleton().GetPrimaryScreen();
    int angle = screen->GetOrientationAngle();
    auto type = screen->GetOrientationType();

    mOwnerThread->Dispatch(NS_NewRunnableFunction(
        "ScreenObserver::UpdateOrientation",
        [angle, type, this, self = RefPtr<ScreenObserver>(this)]() {
          if (mOwner) {
            mOwner->UpdateScreenConfiguration(angle, type);
          }
        }));
  }

  MediaEngineGonkVideoSource* mOwner = nullptr;
  nsCOMPtr<nsISerialEventTarget> mOwnerThread;
};

NS_IMPL_ISUPPORTS(MediaEngineGonkVideoSource::ScreenObserver, nsIObserver)

MediaEngineGonkVideoSource::MediaEngineGonkVideoSource(
    const MediaDevice* aMediaDevice)
    : MediaEngineCameraVideoSource(aMediaDevice),
      mDeviceName(NS_ConvertUTF16toUTF8(aMediaDevice->mRawName)),
      mMutex("MediaEngineGonkVideoSource::mMutex"),
      mRotation(0) {
  MOZ_ASSERT(NS_IsMainThread());
}

MediaEngineGonkVideoSource::~MediaEngineGonkVideoSource() {}

size_t MediaEngineGonkVideoSource::NumCapabilities() const {
  AssertIsOnOwningThread();

  // TODO: Stop hardcoding. Use GetRecorderProfiles+GetProfileInfo (Bug 1128550)
  //
  // The camera-selecting constraints algorithm needs a set of capabilities to
  // work on. In lieu of something better, here are some generic values based on
  // http://en.wikipedia.org/wiki/Comparison_of_Firefox_OS_devices on Jan 2015.
  // When unknown, better overdo it with choices to not block legitimate asks.
  // TODO: Match with actual hardware or add code to query hardware.

  if (mHardcodedCapabilities.IsEmpty()) {
    // Preview sizes supported by GF5:
    const struct {
      int width, height;
    } hardcodes[] = {{1080, 1440}, {960, 1280}, {720, 1280}, {480, 1280},
                     {400, 1280},  {480, 864},  {480, 800},  {432, 768},
                     {480, 720},   {640, 640},  {480, 640},  {640, 480},
                     {360, 640},   {432, 576},  {360, 480},  {320, 480},
                     {288, 384},   {288, 352},  {240, 320},  {320, 240},
                     {160, 240},   {144, 176},  {176, 144},  {120, 160}};
    const int framerates[] = {15, 30};

    for (auto& hardcode : hardcodes) {
      webrtc::CaptureCapability c;
      c.width = hardcode.width;
      c.height = hardcode.height;
      for (int framerate : framerates) {
        c.maxFPS = framerate;
        mHardcodedCapabilities.AppendElement(c);
      }
    }
  }
  return mHardcodedCapabilities.Length();
}

webrtc::CaptureCapability MediaEngineGonkVideoSource::GetCapability(
    size_t aIndex) const {
  MOZ_ASSERT(aIndex < mHardcodedCapabilities.Length());
  return mHardcodedCapabilities.SafeElementAt(aIndex,
                                              webrtc::CaptureCapability());
}

nsresult MediaEngineGonkVideoSource::Allocate(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    uint64_t aWindowID, const char** aOutBadConstraint) {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kReleased);

  NormalizedConstraints constraints(aConstraints);
  webrtc::CaptureCapability newCapability;
  LOG("ChooseCapability(kFitness) for mCapability (Allocate) ++");
  if (!ChooseCapability(constraints, aPrefs, newCapability, kFitness)) {
    *aOutBadConstraint =
        MediaConstraintsHelper::FindBadConstraint(constraints, mMediaDevice);
    return NS_ERROR_FAILURE;
  }

  mScreenObserver = new ScreenObserver(this);

  mWrapper = new CameraControlWrapper(mDeviceName);
  nsresult rv = mWrapper->Allocate(this);
  NS_ENSURE_SUCCESS(rv, rv);

  {
    MutexAutoLock lock(mMutex);
    mState = kAllocated;
    mCapability = newCapability;
  }

  LOG("Video device %s allocated", mDeviceName.Data());
  return NS_OK;
}

nsresult MediaEngineGonkVideoSource::Deallocate() {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kStopped || mState == kAllocated);

  if (mTrack) {
    mTrack->End();
  }

  {
    MutexAutoLock lock(mMutex);

    mTrack = nullptr;
    mPrincipal = PRINCIPAL_HANDLE_NONE;
    mState = kReleased;
  }

  mTextureClientAllocator = nullptr;
  mImageContainer = nullptr;

  mScreenObserver->Shutdown();
  mScreenObserver = nullptr;

  mWrapper->Deallocate();
  mWrapper = nullptr;

  LOG("Video device %s deallocated", mDeviceName.Data());
  return NS_OK;
}

nsresult MediaEngineGonkVideoSource::Start() {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated || mState == kStopped);
  MOZ_ASSERT(mTrack);

  {
    MutexAutoLock lock(mMutex);
    mState = kStarted;
  }

  mSettingsUpdatedByFrame->mValue = false;

  nsresult rv = mWrapper->Start(mCapability);
  if (NS_FAILED(rv)) {
    LOG("Start failed");
    MutexAutoLock lock(mMutex);
    mState = kStopped;
    return rv;
  }

  // Must be called after mWrapper is started
  mScreenObserver->Start();

  RefPtr<MediaEngineGonkVideoSource> self = this;
  NS_DispatchToMainThread(NS_NewRunnableFunction(
      "MediaEngineGonkVideoSource::SetLastCapability",
      [settings = mSettings, updated = mSettingsUpdatedByFrame,
       cap = mCapability]() mutable {
        if (!updated->mValue) {
          settings->mWidth.Value() = cap.width;
          settings->mHeight.Value() = cap.height;
        }
        settings->mFrameRate.Value() = cap.maxFPS;
      }));

  LOG("Video device %s started", mDeviceName.Data());
  return NS_OK;
}

nsresult MediaEngineGonkVideoSource::Stop() {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  if (mState == kStopped || mState == kAllocated) {
    return NS_OK;
  }

  MOZ_ASSERT(mState == kStarted);

  nsresult rv = mWrapper->Stop();
  if (NS_FAILED(rv)) {
    LOG("Stop failed");
    return rv;
  }

  mScreenObserver->Stop();

  {
    MutexAutoLock lock(mMutex);
    mState = kStopped;
  }

  LOG("Video device %s stopped", mDeviceName.Data());
  return NS_OK;
}

nsresult MediaEngineGonkVideoSource::Reconfigure(
    const MediaTrackConstraints& aConstraints, const MediaEnginePrefs& aPrefs,
    const char** aOutBadConstraint) {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  NormalizedConstraints constraints(aConstraints);
  webrtc::CaptureCapability newCapability;
  LOG("ChooseCapability(kFitness) for mTargetCapability (Reconfigure) ++");
  if (!ChooseCapability(constraints, aPrefs, newCapability, kFitness)) {
    *aOutBadConstraint =
        MediaConstraintsHelper::FindBadConstraint(constraints, mMediaDevice);
    return NS_ERROR_INVALID_ARG;
  }
  LOG("ChooseCapability(kFitness) for mTargetCapability (Reconfigure) --");

  if (mCapability == newCapability) {
    return NS_OK;
  }

  bool started = mState == kStarted;
  if (started) {
    nsresult rv = Stop();
    if (NS_WARN_IF(NS_FAILED(rv))) {
      nsAutoCString name;
      GetErrorName(rv, name);
      LOG("Video source %p for video device %s Reconfigure() failed "
          "unexpectedly in Stop(). rv=%s",
          this, mDeviceName.Data(), name.Data());
      return NS_ERROR_UNEXPECTED;
    }
  }

  {
    MutexAutoLock lock(mMutex);
    // Start() applies mCapability on the device.
    mCapability = newCapability;
  }

  if (started) {
    nsresult rv = Start();
    if (NS_WARN_IF(NS_FAILED(rv))) {
      nsAutoCString name;
      GetErrorName(rv, name);
      LOG("Video source %p for video device %s Reconfigure() failed "
          "unexpectedly in Start(). rv=%s",
          this, mDeviceName.Data(), name.Data());
      return NS_ERROR_UNEXPECTED;
    }
  }
  return NS_OK;
}

void MediaEngineGonkVideoSource::SetTrack(const RefPtr<MediaTrack>& aTrack,
                                          const PrincipalHandle& aPrincipal) {
  LOG("%s", __PRETTY_FUNCTION__);
  AssertIsOnOwningThread();

  MOZ_ASSERT(mState == kAllocated);
  MOZ_ASSERT(!mTrack);
  MOZ_ASSERT(aTrack);

  // If WebRender pref is disabled, we can't allocate gralloc texture on our
  // own. In that case, we don't need to create TextureClient allocator.
  if (gfxPlatform::WebRenderPrefEnabled() && !mTextureClientAllocator) {
    RefPtr<layers::ImageBridgeChild> bridge =
        layers::ImageBridgeChild::GetSingleton();
    mTextureClientAllocator = new layers::TextureClientRecycleAllocator(bridge);
    mTextureClientAllocator->SetMaxPoolSize(10);
  }

  if (!mImageContainer) {
    mImageContainer =
        MakeAndAddRef<ImageContainer>(ImageContainer::ASYNCHRONOUS);
  }

  {
    MutexAutoLock lock(mMutex);
    mTrack = aTrack->AsSourceTrack();
    mPrincipal = aPrincipal;
  }
}

static int GetRotateAmount(int aScreenAngle, int aCameraMountAngle,
                           bool aBackCamera) {
  if (aBackCamera) {
    return (aCameraMountAngle - aScreenAngle + 360) % 360;
  } else {
    return (aCameraMountAngle + aScreenAngle) % 360;
  }
}

void MediaEngineGonkVideoSource::UpdateScreenConfiguration(
    int aOrientationAngle, hal::ScreenOrientation aOrientationType) {
  AssertIsOnOwningThread();

  int cameraAngle = mWrapper->GetCameraAngle();
  bool isBackCamera = mWrapper->GetIsBackCamera();
  int rotation = GetRotateAmount(aOrientationAngle, cameraAngle, isBackCamera);
  LOG("Orientation: %d (Camera %s isBackCamera %d MountAngle: %d)", rotation,
      mDeviceName.Data(), isBackCamera, cameraAngle);

  int orientation = 0;
  switch (aOrientationType) {
    case hal::ScreenOrientation::PortraitPrimary:
      orientation = 0;
      break;
    case hal::ScreenOrientation::PortraitSecondary:
      orientation = 180;
      break;
    case hal::ScreenOrientation::LandscapePrimary:
      orientation = 270;
      break;
    case hal::ScreenOrientation::LandscapeSecondary:
      orientation = 90;
      break;
  }
  // Front camera is inverse angle comparing to back camera.
  orientation = (isBackCamera ? orientation : (-orientation));
  mWrapper->SetPhotoOrientation(orientation);

  // Atomic
  mRotation = rotation;
}

nsresult MediaEngineGonkVideoSource::TakePhoto(
    MediaEnginePhotoCallback* aCallback) {
  MOZ_ASSERT(NS_IsMainThread());
  return mWrapper->TakePhoto(aCallback);
}

// When supporting new pixel formats, make sure they are handled in
// |ConvertPixelFormatToFOURCC()|, |IsYUVFormat()| and |ConvertYUVToI420()|.
static uint32_t ConvertPixelFormatToFOURCC(android::PixelFormat aFormat) {
  switch (aFormat) {
    case HAL_PIXEL_FORMAT_RGBA_8888:
      return libyuv::FOURCC_BGRA;
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
#ifdef PRODUCT_MANUFACTURER_SPRD
    case HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED:
#endif
      return libyuv::FOURCC_NV21;
    case HAL_PIXEL_FORMAT_YV12:
      return libyuv::FOURCC_YV12;
    default: {
      LOG("Unknown pixel format %d", aFormat);
      MOZ_ASSERT(false, "Unknown pixel format.");
      return libyuv::FOURCC_ANY;
    }
  }
}

static bool IsYUVFormat(android::PixelFormat aFormat) {
  switch (aFormat) {
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
#ifdef PRODUCT_MANUFACTURER_SPRD
    case HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED:
#endif
    case HAL_PIXEL_FORMAT_YV12:
      return true;
    default:
      return false;
  }
}

// For YUV source, call speciallized libyuv rotate API to handle pixel alignment
// properly. For example, some devices may have a Y plane with 64-pixel
// alignment, so it's necessary to specify the address of each plane separately,
// but the high level API |libyuv::ConvertToI420()| doesn't support it.
static int ConvertYUVToI420(android_ycbcr& aSrcYUV, android_ycbcr& aDstYUV,
                            int aSrcWidth, int aSrcHeight, int aSrcStride,
                            android::PixelFormat aSrcFormat, int aRotation) {
  switch (aSrcFormat) {
    case HAL_PIXEL_FORMAT_YCrCb_420_SP:
#ifdef PRODUCT_MANUFACTURER_SPRD
    case HAL_PIXEL_FORMAT_IMPLEMENTATION_DEFINED:
#endif
      // NV21, but call NV12 rotate API with destination U plane and V plane
      // swapped. Note that for this format, |aSrcYUV.cr| is the starting
      // address of source UV plane, and |aSrcYUV.cr| + 1 == |aSrcYUV.cb|.
      // clang-format off
      return libyuv::NV12ToI420Rotate(
          static_cast<uint8_t*>(aSrcYUV.y),  static_cast<int>(aSrcYUV.ystride),
          static_cast<uint8_t*>(aSrcYUV.cr), static_cast<int>(aSrcYUV.cstride),
          static_cast<uint8_t*>(aDstYUV.y),  static_cast<int>(aDstYUV.ystride),
          static_cast<uint8_t*>(aDstYUV.cr), static_cast<int>(aDstYUV.cstride),
          static_cast<uint8_t*>(aDstYUV.cb), static_cast<int>(aDstYUV.cstride),
          aSrcWidth, aSrcHeight, static_cast<libyuv::RotationMode>(aRotation));
      // clang-format on
    case HAL_PIXEL_FORMAT_YV12:
      // clang-format off
      return libyuv::I420Rotate(
          static_cast<uint8_t*>(aSrcYUV.y),  static_cast<int>(aSrcYUV.ystride),
          static_cast<uint8_t*>(aSrcYUV.cb), static_cast<int>(aSrcYUV.cstride),
          static_cast<uint8_t*>(aSrcYUV.cr), static_cast<int>(aSrcYUV.cstride),
          static_cast<uint8_t*>(aDstYUV.y),  static_cast<int>(aDstYUV.ystride),
          static_cast<uint8_t*>(aDstYUV.cb), static_cast<int>(aDstYUV.cstride),
          static_cast<uint8_t*>(aDstYUV.cr), static_cast<int>(aDstYUV.cstride),
          aSrcWidth, aSrcHeight, static_cast<libyuv::RotationMode>(aRotation));
      // clang-format on
    default:
      LOG("Unknown YUV format %d", aSrcFormat);
      MOZ_ASSERT(false, "Unknown YUV format.");
      return -1;
  }
}

static int RotateBuffer(sp<GraphicBuffer> aSrcBuffer, android_ycbcr& aDstYUV,
                        int aRotation) {
  int ret = -1;
  android::PixelFormat srcFormat = aSrcBuffer->getPixelFormat();
  if (IsYUVFormat(srcFormat)) {
    // Specialized path that handles source YUV alignment more properly.
    android_ycbcr srcYUV = {};
    aSrcBuffer->lockYCbCr(GraphicBuffer::USAGE_SW_READ_MASK, &srcYUV);
    ret = ConvertYUVToI420(srcYUV, aDstYUV, aSrcBuffer->getWidth(),
                           aSrcBuffer->getHeight(), aSrcBuffer->getStride(),
                           srcFormat, aRotation);
  } else {
    // Generalized path that calls libyuv's high level API.
    void* srcAddr = nullptr;
    aSrcBuffer->lock(GraphicBuffer::USAGE_SW_READ_MASK, &srcAddr);
    int srcWidth = aSrcBuffer->getWidth();
    int srcHeight = aSrcBuffer->getHeight();
    int srcStride = aSrcBuffer->getStride();
    // clang-format off
    ret = libyuv::ConvertToI420(
        static_cast<uint8_t*>(srcAddr), 0,
        static_cast<uint8_t*>(aDstYUV.y),  static_cast<int>(aDstYUV.ystride),
        static_cast<uint8_t*>(aDstYUV.cb), static_cast<int>(aDstYUV.cstride),
        static_cast<uint8_t*>(aDstYUV.cr), static_cast<int>(aDstYUV.cstride),
        0, 0, srcStride, srcHeight, srcWidth, srcHeight,
        static_cast<libyuv::RotationMode>(aRotation),
        ConvertPixelFormatToFOURCC(srcFormat));
    // clang-format on
  }
  aSrcBuffer->unlock();
  return ret;
}

static int RotateBuffer(sp<GraphicBuffer> aSrcBuffer,
                        sp<GraphicBuffer> aDstBuffer, int aRotation) {
  android_ycbcr dstYUV = {};
  aDstBuffer->lockYCbCr(GraphicBuffer::USAGE_SW_WRITE_OFTEN, &dstYUV);
  int ret = RotateBuffer(aSrcBuffer, dstYUV, aRotation);
  aDstBuffer->unlock();
  return ret;
}

static int RotateBuffer(sp<GraphicBuffer> aSrcBuffer,
                        layers::MappedYCbCrTextureData& aDstBuffer,
                        int aRotation) {
  android_ycbcr dstYUV = {.y = aDstBuffer.y.data,
                          .cb = aDstBuffer.cb.data,
                          .cr = aDstBuffer.cr.data,
                          .ystride = static_cast<size_t>(aDstBuffer.y.stride),
                          .cstride = static_cast<size_t>(aDstBuffer.cb.stride),
                          .chroma_step = 1};

  return RotateBuffer(aSrcBuffer, dstYUV, aRotation);
}

already_AddRefed<layers::Image>
MediaEngineGonkVideoSource::CreateI420GrallocImage(uint32_t aWidth,
                                                   uint32_t aHeight) {
  if (!mTextureClientAllocator) {
    return nullptr;
  }

  RefPtr<layers::TextureClient> textureClient =
      mTextureClientAllocator->CreateOrRecycle(
          gfx::SurfaceFormat::YUV, IntSize(aWidth, aHeight),
          layers::BackendSelector::Content, layers::TextureFlags::DEFAULT,
          layers::ALLOC_DISALLOW_BUFFERTEXTURECLIENT);
  if (!textureClient) {
    return nullptr;
  }

  if (!textureClient->GetInternalData()->AsGrallocTextureData()) {
    return nullptr;
  }

  RefPtr<layers::GrallocImage> image = new layers::GrallocImage();
  image->AdoptData(textureClient, IntSize(aWidth, aHeight));
  return image.forget();
}

already_AddRefed<layers::Image>
MediaEngineGonkVideoSource::CreateI420PlanarYCbCrImage(uint32_t aWidth,
                                                       uint32_t aHeight) {
  if (!mImageContainer) {
    return nullptr;
  }

  RefPtr<layers::PlanarYCbCrImage> image =
      mImageContainer->CreatePlanarYCbCrImage();
  if (!image) {
    return nullptr;
  }

  auto ySize = IntSize(aWidth, aHeight);
  auto uvSize = IntSize((aWidth + 1) / 2, (aHeight + 1) / 2);

  layers::PlanarYCbCrData data;
  data.mYStride = ySize.Width();
  data.mCbCrStride = uvSize.Width();
  data.mPictureRect = IntRect(0, 0, aWidth, aHeight);
  data.mChromaSubsampling = gfx::ChromaSubsampling::HALF_WIDTH_AND_HEIGHT;
  data.mColorDepth = gfx::ColorDepth::COLOR_8;
  if (!image->CreateEmptyBuffer(data, ySize, uvSize)) {
    return nullptr;
  }
  return image.forget();
}

// Rotate the source image and convert it to I420 format, which is mandatory in
// WebRTC library.
already_AddRefed<layers::Image> MediaEngineGonkVideoSource::RotateImage(
    layers::Image* aImage, uint32_t aWidth, uint32_t aHeight) {
  sp<GraphicBuffer> srcBuffer = aImage->AsGrallocImage()->GetGraphicBuffer();

  uint32_t dstWidth = aWidth;
  uint32_t dstHeight = aHeight;
  int rotation = mRotation;
  if (rotation == 90 || rotation == 270) {
    std::swap(dstWidth, dstHeight);
  }

  RefPtr<layers::Image> image;
  if (image = CreateI420GrallocImage(dstWidth, dstHeight)) {
    sp<GraphicBuffer> dstBuffer = image->AsGrallocImage()->GetGraphicBuffer();
    if (RotateBuffer(srcBuffer, dstBuffer, rotation) == -1) {
      return nullptr;
    }
  } else if (image = CreateI420PlanarYCbCrImage(dstWidth, dstHeight)) {
    RefPtr<layers::TextureClient> textureClient =
        image->GetTextureClient(nullptr);

    layers::TextureClientAutoLock autoLock(textureClient,
                                           layers::OpenMode::OPEN_WRITE_ONLY);
    if (!autoLock.Succeeded()) {
      return nullptr;
    }

    layers::MappedYCbCrTextureData dstBuffer;
    if (!textureClient->BorrowMappedYCbCrData(dstBuffer)) {
      return nullptr;
    }

    if (RotateBuffer(srcBuffer, dstBuffer, rotation) == -1) {
      return nullptr;
    }

    textureClient->MarkImmutable();
  }
  return image.forget();
}

// CameraControlWrapper is holding mMonitor, so be careful with mutex locking.
bool MediaEngineGonkVideoSource::OnNewPreviewFrame(layers::Image* aImage,
                                                   uint32_t aWidth,
                                                   uint32_t aHeight) {
  // Bug XXX we'd prefer to avoid converting if mRotation == 0, but that causes
  // problems in UpdateImage()
  RefPtr<layers::Image> rotatedImage = RotateImage(aImage, aWidth, aHeight);
  IntSize rotatedSize = rotatedImage->GetSize();

  if (mImageSize != rotatedSize) {
    NS_DispatchToMainThread(NS_NewRunnableFunction(
        "MediaEngineGonkVideoSource::FrameSizeChange",
        [settings = mSettings, updated = mSettingsUpdatedByFrame,
         holder = std::move(mFirstFramePromiseHolder), rotatedSize]() mutable {
          settings->mWidth.Value() = rotatedSize.width;
          settings->mHeight.Value() = rotatedSize.height;
          updated->mValue = true;
          // Since mImageSize was initialized to (0,0), we end up here on the
          // arrival of the first frame. We resolve the promise representing
          // arrival of first frame, after correct settings values have been
          // made available (Resolve() is idempotent if already resolved).
          holder.ResolveIfExists(true, __func__);
        }));
  }

  {
    MutexAutoLock lock(mMutex);
    VideoSegment segment;
    mImageSize = rotatedSize;
    segment.AppendFrame(rotatedImage.forget(), rotatedSize, mPrincipal);
    mTrack->AppendData(&segment);
  }
  return true;  // return true because we're accepting the frame
}

}  // namespace mozilla
