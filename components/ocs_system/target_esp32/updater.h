/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_ota_ops.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/icrc32_calculator.h"
#include "ocs_system/iupdater.h"

namespace ocs {
namespace system {

class Updater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p crc32_calculator to calculate CRC32 checksum to check data integrity.
    explicit Updater(ICrc32Calculator& crc32_calculator);

    //! Begin firmware update.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit firmware update.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    ICrc32Calculator& crc32_calculator_;

    uint32_t crc32_src_ { 0 };
    uint32_t crc32_clc_ { 0 };
    bool committed_ { false };

    esp_ota_handle_t handle_ { 0 };
};

} // namespace system
} // namespace ocs
