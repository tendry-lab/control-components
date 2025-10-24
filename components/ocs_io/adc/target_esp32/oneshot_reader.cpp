/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_io/adc/target_esp32/oneshot_reader.h"
#include "ocs_core/log.h"

namespace ocs {
namespace io {
namespace adc {

namespace {

const char* log_tag = "adc_oneshot_reader";

} // namespace

OneshotReader::OneshotReader(Channel channel, adc_oneshot_unit_handle_t unit_handle)
    : channel_(channel)
    , unit_handle_(unit_handle) {
}

status::StatusCode OneshotReader::read(int& raw) {
    const auto err = adc_oneshot_read(unit_handle_, channel_, &raw);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_oneshot_read(): channel=%u err=%s", channel_,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace adc
} // namespace io
} // namespace ocs
