/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_system/crc32_updater.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "crc32_updater";

} // namespace

Crc32Updater::Crc32Updater(ICrc32Calculator& crc32_calculator, IUpdater& updater)
    : crc32_calculator_(crc32_calculator)
    , updater_(updater) {
}

status::StatusCode Crc32Updater::begin(size_t total_size, uint32_t crc32) {
    configASSERT(!crc32_src_);
    configASSERT(!crc32_clc_);

    crc32_src_ = crc32;

    const auto code = updater_.begin(total_size, crc32);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode Crc32Updater::write(const uint8_t* buf, size_t len) {
    const auto code = updater_.write(buf, len);
    if (code != status::StatusCode::OK) {
        return code;
    }

    crc32_clc_ = crc32_calculator_.calculate(crc32_clc_, buf, len);

    return status::StatusCode::OK;
}

status::StatusCode Crc32Updater::commit() {
    configASSERT(crc32_src_);
    configASSERT(crc32_clc_);

    if (crc32_src_ != crc32_clc_) {
        ocs_loge(log_tag, "integrity check failed: crc32 mismatch: want=%lu got=%lu",
                 crc32_src_, crc32_clc_);

        return status::StatusCode::Error;
    }

    const auto code = updater_.commit();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode Crc32Updater::end() {
    crc32_src_ = 0;
    crc32_clc_ = 0;

    const auto code = updater_.end();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
