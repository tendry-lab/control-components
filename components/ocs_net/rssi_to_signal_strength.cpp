/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
