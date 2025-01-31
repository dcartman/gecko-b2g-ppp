/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

dictionary NetworkInterface {
  long type;
  DOMString id;
};

/**
 * Represents a data interface for which the manager is recording statistics.
 */
[Exposed=Window,
 JSImplementation="@mozilla.org/networkstatsinterface;1",
 Pref="dom.networkStats.enabled"]
interface NetworkStatsInterface {
  constructor(optional NetworkInterface networkinterface = {});

  readonly attribute long type;

  /**
   * Id value is '0' for wifi or the iccid for mobile (SIM).
   */
  readonly attribute DOMString id;

  [Default] object toJSON();
};
