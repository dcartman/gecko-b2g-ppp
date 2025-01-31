/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_CookieBannerDomainPrefService_h__
#define mozilla_CookieBannerDomainPrefService_h__

#include "nsIContentPrefService2.h"

#include "mozilla/Maybe.h"
#include "nsStringFwd.h"
#include "nsTHashMap.h"
#include "nsTHashSet.h"

#include "nsICookieBannerService.h"
#include "nsIObserver.h"
#include "nsIAsyncShutdown.h"

namespace mozilla {

// The service which maintains the per-domain cookie banner preference. It uses
// the content pref to store the per-domain preference for cookie banner
// handling. To support the synchronous access, the service caches the
// preferences in the memory.
class CookieBannerDomainPrefService final : public nsIAsyncShutdownBlocker,
                                            public nsIObserver {
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIASYNCSHUTDOWNBLOCKER
  NS_DECL_NSIOBSERVER

  static already_AddRefed<CookieBannerDomainPrefService> GetOrCreate();

  // Get the preference for the given domain.
  Maybe<nsICookieBannerService::Modes> GetPref(const nsACString& aDomain,
                                               bool aIsPrivate);

  // Set the preference for the given domain.
  [[nodiscard]] nsresult SetPref(const nsACString& aDomain,
                                 nsICookieBannerService::Modes aMode,
                                 bool aIsPrivate);

  // Remove the preference for the given domain.
  [[nodiscard]] nsresult RemovePref(const nsACString& aDomain, bool aIsPrivate);

  // Remove all site preferences.
  [[nodiscard]] nsresult RemoveAll(bool aIsPrivate);

  void Init();

 private:
  ~CookieBannerDomainPrefService() = default;

  CookieBannerDomainPrefService()
      : mIsInitialized(false),
        mIsContentPrefLoaded(false),
        mIsShuttingDown(false) {}

  // Indicates whether the service is initialized.
  bool mIsInitialized;

  // Indicates whether the first reading of content pref completed.
  bool mIsContentPrefLoaded;

  // Indicates whether we are shutting down.
  bool mIsShuttingDown;

  // Map of the per site preference keyed by domain.
  nsTHashMap<nsCStringHashKey, nsICookieBannerService::Modes> mPrefs;

  // Map of the per site preference for private windows keyed by domain.
  nsTHashMap<nsCStringHashKey, nsICookieBannerService::Modes> mPrefsPrivate;

  // A helper function that will wait until the initialization of the content
  // pref completed.
  void EnsureInitCompleted();

  nsresult AddShutdownBlocker();
  nsresult RemoveShutdownBlocker();

  class BaseContentPrefCallback : public nsIContentPrefCallback2 {
   public:
    NS_DECL_ISUPPORTS

    NS_IMETHOD HandleResult(nsIContentPref*) override = 0;
    NS_IMETHOD HandleCompletion(uint16_t) override = 0;
    NS_IMETHOD HandleError(nsresult) override = 0;

    explicit BaseContentPrefCallback(CookieBannerDomainPrefService* aService)
        : mService(aService) {}

   protected:
    virtual ~BaseContentPrefCallback() = default;
    RefPtr<CookieBannerDomainPrefService> mService;
  };

  class InitialLoadContentPrefCallback final : public BaseContentPrefCallback {
   public:
    NS_DECL_NSICONTENTPREFCALLBACK2

    explicit InitialLoadContentPrefCallback(
        CookieBannerDomainPrefService* aService)
        : BaseContentPrefCallback(aService) {}
  };

  class WriteContentPrefCallback final : public BaseContentPrefCallback {
   public:
    NS_DECL_NSICONTENTPREFCALLBACK2

    explicit WriteContentPrefCallback(CookieBannerDomainPrefService* aService)
        : BaseContentPrefCallback(aService) {}
  };

  // A counter to track if there is any writing is happening. We will use this
  // to decide if we can remove the shutdown blocker.
  uint32_t mWritingCount = 0;

  void Shutdown();
};

}  // namespace mozilla

#endif  // mozilla_CookieBannerDomainPrefService_h__
