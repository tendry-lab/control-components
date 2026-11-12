/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_partition.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/iblob_writer.h"

namespace ocs {
namespace system {

//! Write the blob to the partition.
class PartitionBlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p type - partition type to write to.
    //!  - @p subtype - partition subtype to write to.
    //!  - @p partname - partition name to write to.
    PartitionBlobWriter(esp_partition_type_t type,
                        esp_partition_subtype_t subtype,
                        const char* partname);

    //! Erase the partition.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write blob data to the partition.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit the blob.
    status::StatusCode commit() override;

    //! End writing the blob.
    status::StatusCode end() override;

private:
    const esp_partition_t* partition_ { nullptr };

    size_t offset_ { 0 };
};

} // namespace system
} // namespace ocs
