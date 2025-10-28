/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_storage/ifs_initializer.h"

namespace ocs {
namespace storage {

class SpiffsInitializer : public IFsInitializer, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p mount_point - SPIFFS mount point.
    explicit SpiffsInitializer(const char* mount_point);

    //! Return true if SPIFFS has been properly initialized and mounted to the VFS.
    bool valid() const override;

private:
    bool valid_ { false };
};

} // namespace storage
} // namespace ocs
