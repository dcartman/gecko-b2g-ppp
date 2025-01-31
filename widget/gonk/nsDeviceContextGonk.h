/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef nsDeviceContextGonk_h__
#define nsDeviceContextGonk_h__

#include "nsIDeviceContextSpec.h"
#include "nsCOMPtr.h"

class nsDeviceContextSpecGonk final : public nsIDeviceContextSpec {
 private:
  virtual ~nsDeviceContextSpecGonk();

 public:
  NS_DECL_ISUPPORTS

  already_AddRefed<PrintTarget> MakePrintTarget() final;

  NS_IMETHOD Init(nsIPrintSettings* aPS, bool aIsPrintPreview) override;
  NS_IMETHOD BeginDocument(const nsAString& aTitle,
                           const nsAString& aPrintToFileName,
                           int32_t aStartPage, int32_t aEndPage) override;
  NS_IMETHOD EndDocument() override;
  NS_IMETHOD BeginPage() override { return NS_OK; }
  NS_IMETHOD EndPage() override { return NS_OK; }

 private:
  nsCOMPtr<nsIPrintSettings> mPrintSettings;
  nsCOMPtr<nsIFile> mTempFile;
};
#endif  // nsDeviceContextGonk_h__
