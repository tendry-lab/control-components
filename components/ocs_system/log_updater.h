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

class LogUpdater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p updater to perform the actual update process.
    //!  - @p log_tag to use for logging.
    LogUpdater(IUpdater& updater, const char* log_tag);

    //! Begin firmware update.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit firmware update.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    const std::string log_tag_;

    IUpdater& updater_;
};

} // namespace system
} // namespace ocs
