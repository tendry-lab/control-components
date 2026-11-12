/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_ota_ops.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/iblob_writer.h"

namespace ocs {
namespace system {

//! Write the firmware image to the next OTA partition.
//!
//! @remarks
//!  end() after a successful commit() makes the written partition the boot one.
class OtaBlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Begin writing the firmware image.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write firmware image data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit the firmware image.
    status::StatusCode commit() override;

    //! Set the boot partition if committed, abort the OTA otherwise.
    status::StatusCode end() override;

private:
    bool committed_ { false };

    esp_ota_handle_t handle_ { 0 };
};

} // namespace system
} // namespace ocs
