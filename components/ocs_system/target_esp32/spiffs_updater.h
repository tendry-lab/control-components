/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_system/iupdater.h"

namespace ocs {
namespace system {

class SpiffsUpdater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p updater - to perform the actual update process.
    //!  - @p partition_label - SPIFFS partition label.
    SpiffsUpdater(IUpdater& updater, const char* partition_label);

    //! Unmount SPIFFS partition and begin the firmware update process.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit firmware update.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    const std::string partition_label_;

    IUpdater& updater_;
};

} // namespace system
} // namespace ocs
