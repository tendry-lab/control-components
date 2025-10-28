/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace storage {

class IFsInitializer {
public:
    //! Return true if FS has been properly initialized and mounted to the VFS.
    virtual bool valid() const = 0;
};

} // namespace storage
} // namespace ocs
