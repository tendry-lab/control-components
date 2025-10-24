/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdio>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_onewire/serial_number_to_str.h"

namespace ocs {
namespace onewire {

serial_number_to_str::serial_number_to_str(const SerialNumber& serial_number) {
    memset(buf_, 0, sizeof(buf_));

    configASSERT(snprintf(buf_, sizeof(buf_), DS_SENSOR_SERIAL_NUMBER_STR,
                          DS_SENSOR_SERIAL_NUMBER_TO_STR(serial_number))
                 == str_length);
}

const char* serial_number_to_str::c_str() const {
    return buf_;
}

} // namespace onewire
} // namespace ocs
