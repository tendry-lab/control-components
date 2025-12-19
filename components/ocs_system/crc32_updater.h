/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/icrc32_calculator.h"
#include "ocs_system/iupdater.h"

namespace ocs {
namespace system {

class Crc32Updater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p crc32_calculator to calculate CRC32 checksum to check data integrity.
    //!  - @p updater to perform the actual update process.
    Crc32Updater(ICrc32Calculator& crc32_calculator, IUpdater& updater);

    //! Begin firmware update.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Calculate CRC32 and commit firmware update.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    ICrc32Calculator& crc32_calculator_;
    IUpdater& updater_;

    uint32_t crc32_src_ { 0 };
    uint32_t crc32_clc_ { 0 };
};

} // namespace system
} // namespace ocs
