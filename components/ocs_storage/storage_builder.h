/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <set>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace storage {

class StorageBuilder : private core::NonCopyable<> {
public:
    using IStoragePtr = std::unique_ptr<IStorage>;

    //! Create a storage with a unique @ id.
    //!
    //! @return
    //!  nullptr if storage with @p id already exists.
    IStoragePtr make(const char* id);

private:
    std::set<std::string> ids_;
};

} // namespace storage
} // namespace ocs
