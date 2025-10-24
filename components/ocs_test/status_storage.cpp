/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/status_storage.h"

namespace ocs {
namespace test {

StatusStorage::StatusStorage(storage::IStorage& storage)
    : storage_(storage) {
}

status::StatusCode StatusStorage::probe(const char* key, size_t& size) {
    if (probe_status != status::StatusCode::OK) {
        return probe_status;
    }

    return storage_.probe(key, size);
}

status::StatusCode StatusStorage::read(const char* key, void* value, size_t size) {
    if (read_status != status::StatusCode::OK) {
        return read_status;
    }

    return storage_.read(key, value, size);
}

status::StatusCode StatusStorage::write(const char* key, const void* value, size_t size) {
    if (write_status != status::StatusCode::OK) {
        return write_status;
    }

    return storage_.write(key, value, size);
}

status::StatusCode StatusStorage::erase(const char* key) {
    if (erase_status != status::StatusCode::OK) {
        return erase_status;
    }

    return storage_.erase(key);
}

status::StatusCode StatusStorage::erase_all() {
    if (erase_all_status != status::StatusCode::OK) {
        return erase_all_status;
    }

    return storage_.erase_all();
}

} // namespace test
} // namespace ocs
