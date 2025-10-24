/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdint>

#pragma once

namespace ocs {
namespace net {

//! Conver RSSI value into human-readable signal strength.
const char* rssi_to_signal_strength(int rssi);

} // namespace net
} // namespace ocs
