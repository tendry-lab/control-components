/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_partition.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/iupdater.h"

namespace ocs {
namespace system {

class CacheUpdater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p updater to perform the actual update process.
    //!  - @p chunk_size to use when writing data from the cached partition.
    CacheUpdater(IUpdater& updater, size_t chunk_size);

    //! Begin firmware update.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data to the cached partition.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Write firmware data from the cached partition to the updating partition.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    status::StatusCode write_();

    const size_t chunk_size_ { 0 };

    IUpdater& updater_;

    const esp_partition_t* partition_ { nullptr };
    size_t total_size_ { 0 };
    size_t crc32_ { 0 };
    size_t offset_ { 0 };
};

} // namespace system
} // namespace ocs
