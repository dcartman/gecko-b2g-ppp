/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "BluetoothReplyRunnable.h"
#include "BluetoothService.h"
#include "BluetoothUtils.h"
#include "mozilla/dom/BluetoothGattCharacteristicBinding.h"
#include "mozilla/dom/BluetoothGattDescriptorBinding.h"
#include "mozilla/dom/bluetooth/BluetoothCommon.h"
#include "mozilla/dom/bluetooth/BluetoothGattCharacteristic.h"
#include "mozilla/dom/bluetooth/BluetoothGattDescriptor.h"
#include "mozilla/dom/bluetooth/BluetoothGattService.h"
#include "mozilla/dom/bluetooth/BluetoothTypes.h"

using namespace mozilla;
using namespace mozilla::dom;

USING_BLUETOOTH_NAMESPACE

NS_IMPL_CYCLE_COLLECTION_WRAPPERCACHE_CLASS(BluetoothGattDescriptor)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(BluetoothGattDescriptor)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mOwner)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mCharacteristic)
  NS_IMPL_CYCLE_COLLECTION_UNLINK_PRESERVED_WRAPPER

  /**
   * Unregister the bluetooth signal handler after unlinked.
   *
   * This is needed to avoid ending up with exposing a deleted object to JS or
   * accessing deleted objects while receiving signals from parent process
   * after unlinked. Please see Bug 1138267 for detail informations.
   */
  nsString path;
  GeneratePathFromHandle(tmp->mDescriptorHandle, path);
  UnregisterBluetoothSignalHandler(path, tmp);
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(BluetoothGattDescriptor)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mOwner)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mCharacteristic)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTING_ADDREF(BluetoothGattDescriptor)
NS_IMPL_CYCLE_COLLECTING_RELEASE(BluetoothGattDescriptor)
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(BluetoothGattDescriptor)
  NS_WRAPPERCACHE_INTERFACE_MAP_ENTRY
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

const uint16_t BluetoothGattDescriptor::sHandleCount = 1;

// Constructor of BluetoothGattDescriptor in ATT client role
BluetoothGattDescriptor::BluetoothGattDescriptor(
    nsPIDOMWindowInner* aOwner, BluetoothGattCharacteristic* aCharacteristic,
    const BluetoothGattDbElement& aDbElement)
    : mOwner(aOwner),
      mCharacteristic(aCharacteristic),
      mUuid(aDbElement.mUuid),
      mInstanceId(aDbElement.mId),
      mPermissions(BLUETOOTH_EMPTY_GATT_ATTR_PERM),
      mAttRole(ATT_CLIENT_ROLE),
      mActive(true),
      mDescriptorHandle(aDbElement.mHandle) {
  MOZ_ASSERT(aOwner);
  MOZ_ASSERT(aCharacteristic);

  UuidToString(aDbElement.mUuid, mUuidStr);

  // Generate bluetooth signal path of this descriptor to applications
  nsString path;
  GeneratePathFromHandle(mDescriptorHandle, path);
  RegisterBluetoothSignalHandler(path, this);
}

// Constructor of BluetoothGattDescriptor in ATT server role
BluetoothGattDescriptor::BluetoothGattDescriptor(
    nsPIDOMWindowInner* aOwner, BluetoothGattCharacteristic* aCharacteristic,
    const nsAString& aDescriptorUuid, const GattPermissions& aPermissions,
    const ArrayBuffer& aValue)
    : mOwner(aOwner),
      mCharacteristic(aCharacteristic),
      mUuidStr(aDescriptorUuid),
      mPermissions(BLUETOOTH_EMPTY_GATT_ATTR_PERM),
      mAttRole(ATT_SERVER_ROLE),
      mActive(false) {
  MOZ_ASSERT(aOwner);
  MOZ_ASSERT(aCharacteristic);

  // UUID
  StringToUuid(aDescriptorUuid, mUuid);

  // permissions
  GattPermissionsToBits(aPermissions, mPermissions);

  // value
  aValue.ComputeState();
  mValue.AppendElements(aValue.Data(), aValue.Length());
}

BluetoothGattDescriptor::~BluetoothGattDescriptor() {
  nsString path;
  GeneratePathFromHandle(mDescriptorHandle, path);
  UnregisterBluetoothSignalHandler(path, this);
}

void BluetoothGattDescriptor::HandleDescriptorValueUpdated(
    const BluetoothValue& aValue) {
  MOZ_ASSERT(aValue.type() == BluetoothValue::TArrayOfuint8_t);

  mValue = aValue.get_ArrayOfuint8_t().Clone();
}

void BluetoothGattDescriptor::AssignDescriptorHandle(
    const BluetoothAttributeHandle& aDescriptorHandle) {
  MOZ_ASSERT(mAttRole == ATT_SERVER_ROLE);
  MOZ_ASSERT(!mActive);
  MOZ_ASSERT(!mDescriptorHandle.mHandle);

  mDescriptorHandle = aDescriptorHandle;
  mActive = true;
}

void BluetoothGattDescriptor::Notify(const BluetoothSignal& aData) {
  BT_LOGD("[D] %s", NS_ConvertUTF16toUTF8(aData.name()).get());
  NS_ENSURE_TRUE_VOID(mSignalRegistered);

  BluetoothValue v = aData.value();
  if (aData.name().EqualsLiteral("DescriptorValueUpdated")) {
    HandleDescriptorValueUpdated(v);
  } else {
    BT_WARNING("Not handling GATT Descriptor signal: %s",
               NS_ConvertUTF16toUTF8(aData.name()).get());
  }
}

void BluetoothGattDescriptor::GetUuid(BluetoothUuid& aUuid) const {
  aUuid = mUuid;
}

JSObject* BluetoothGattDescriptor::WrapObject(
    JSContext* aContext, JS::Handle<JSObject*> aGivenProto) {
  return BluetoothGattDescriptor_Binding::Wrap(aContext, this, aGivenProto);
}

void BluetoothGattDescriptor::GetValue(
    JSContext* cx, JS::MutableHandle<JSObject*> aValue) const {
  aValue.set(mValue.IsEmpty()
                 ? nullptr
                 : ArrayBuffer::Create(cx, mValue.Length(), mValue.Elements()));
}

void BluetoothGattDescriptor::GetPermissions(
    GattPermissions& aPermissions) const {
  GattPermissionsToDictionary(mPermissions, aPermissions);
}

// Use this namespace to prevent redefinition caused by UNIFIED_SOURCES
namespace descriptor {
class ReadValueTask final : public BluetoothReplyRunnable {
 public:
  ReadValueTask(BluetoothGattDescriptor* aDescriptor, Promise* aPromise)
      : BluetoothReplyRunnable(nullptr, aPromise), mDescriptor(aDescriptor) {
    MOZ_ASSERT(aDescriptor);
    MOZ_ASSERT(aPromise);
  }

  bool ParseSuccessfulReply(JS::MutableHandle<JS::Value> aValue) {
    aValue.setUndefined();

    const BluetoothValue& v = mReply->get_BluetoothReplySuccess().value();
    NS_ENSURE_TRUE(v.type() == BluetoothValue::TArrayOfuint8_t, false);

    AutoJSAPI jsapi;
    NS_ENSURE_TRUE(jsapi.Init(mDescriptor->GetParentObject()), false);

    JSContext* cx = jsapi.cx();
    if (!ToJSValue(cx, v.get_ArrayOfuint8_t(), aValue)) {
      jsapi.ClearException();
      return false;
    }

    return true;
  }

  void ReleaseMembers() {
    BluetoothReplyRunnable::ReleaseMembers();
    mDescriptor = nullptr;
  }

 private:
  RefPtr<BluetoothGattDescriptor> mDescriptor;
};
}  // namespace descriptor

already_AddRefed<Promise> BluetoothGattDescriptor::ReadValue(ErrorResult& aRv) {
  nsCOMPtr<nsIGlobalObject> global = do_QueryInterface(GetParentObject());
  if (!global) {
    aRv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  RefPtr<Promise> promise = Promise::Create(global, aRv);
  NS_ENSURE_TRUE(!aRv.Failed(), nullptr);

  BluetoothUuid appUuid;
  BT_ENSURE_TRUE_REJECT(NS_SUCCEEDED(StringToUuid(
                            mCharacteristic->Service()->GetAppUuid(), appUuid)),
                        promise, NS_ERROR_DOM_OPERATION_ERR);

  if (mAttRole == ATT_SERVER_ROLE) {
    promise->MaybeResolve(mValue);
    return promise.forget();
  }

  BluetoothService* bs = BluetoothService::Get();
  BT_ENSURE_TRUE_REJECT(bs, promise, NS_ERROR_NOT_AVAILABLE);

  bs->GattClientReadDescriptorValueInternal(
      appUuid, mDescriptorHandle, new descriptor::ReadValueTask(this, promise));

  return promise.forget();
}

already_AddRefed<Promise> BluetoothGattDescriptor::WriteValue(
    const ArrayBuffer& aValue, ErrorResult& aRv) {
  nsCOMPtr<nsIGlobalObject> global = do_QueryInterface(GetParentObject());
  if (!global) {
    aRv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  RefPtr<Promise> promise = Promise::Create(global, aRv);
  NS_ENSURE_TRUE(!aRv.Failed(), nullptr);

  BluetoothUuid appUuid;
  BT_ENSURE_TRUE_REJECT(NS_SUCCEEDED(StringToUuid(
                            mCharacteristic->Service()->GetAppUuid(), appUuid)),
                        promise, NS_ERROR_DOM_OPERATION_ERR);

  aValue.ComputeState();

  if (mAttRole == ATT_SERVER_ROLE) {
    mValue.Clear();
    mValue.AppendElements(aValue.Data(), aValue.Length());

    promise->MaybeResolve(JS::UndefinedHandleValue);
    return promise.forget();
  }

  nsTArray<uint8_t> value;
  value.AppendElements(aValue.Data(), aValue.Length());

  BluetoothService* bs = BluetoothService::Get();
  BT_ENSURE_TRUE_REJECT(bs, promise, NS_ERROR_NOT_AVAILABLE);

  bs->GattClientWriteDescriptorValueInternal(
      appUuid, mDescriptorHandle, value,
      new BluetoothVoidReplyRunnable(nullptr, promise));

  return promise.forget();
}
