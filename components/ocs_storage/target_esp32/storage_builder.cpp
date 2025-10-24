/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_storage/storage_builder.h"
#include "ocs_storage/target_esp32/nvs_storage.h"

namespace ocs {
namespace storage {

StorageBuilder::IStoragePtr StorageBuilder::make(const char* id) {
    auto [it, ok] = ids_.insert(id);
    if (!ok) {
        return nullptr;
    }

    return StorageBuilder::IStoragePtr(new (std::nothrow) NvsStorage(id));
}

} // namespace storage
} // namespace ocs
