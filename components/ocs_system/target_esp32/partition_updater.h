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

class PartitionUpdater : public IUpdater, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p type - partition type being updated.
    //!  - @p subtype - partition subtype being updated.
    //!  - @p partname - partition name being updated.
    PartitionUpdater(esp_partition_type_t type,
                     esp_partition_subtype_t subtype,
                     const char* partname);

    //! Begin firmware update.
    status::StatusCode begin(size_t total_size, uint32_t crc32) override;

    //! Write firmware data to the updating partition.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit written firmware data.
    status::StatusCode commit() override;

    //! End firmware update.
    status::StatusCode end() override;

private:
    const esp_partition_t* partition_ { nullptr };

    size_t offset_ { 0 };
};

} // namespace system
} // namespace ocs
