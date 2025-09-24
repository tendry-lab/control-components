/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/rssi_to_signal_strength.h"

namespace ocs {
namespace net {

const char* rssi_to_signal_strength(int rssi) {
    if (rssi > -50) {
        return "Excellent";
    }

    if (rssi > -60) {
        return "Good";
    }

    if (rssi > -70) {
        return "Fair";
    }

    if (rssi > -80) {
        return "Poor";
    }

    return "Unreliable";
}

} // namespace net
} // namespace ocs
