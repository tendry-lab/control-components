/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

#define SHT_SENSOR_SERIAL_NUMBER_LEN 4

#define SHT_SENSOR_SERIAL_NUMBER_STR "%02X:%02X:%02X:%02X"

#define SHT_SENSOR_SERIAL_NUMBER_TO_STR(serial_number)                                   \
    serial_number[0], serial_number[1], serial_number[2], serial_number[3]

namespace ocs {
namespace sensor {
namespace sht4x {

//! SHT sensor serial number, MSB byte order.
using SerialNumber = uint8_t[SHT_SENSOR_SERIAL_NUMBER_LEN];

} // namespace sht4x
} // namespace sensor
} // namespace ocs
