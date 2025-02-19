/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_test/memory_storage.h"

namespace ocs {
namespace test {

status::StatusCode MemoryStorage::probe(const char* key, size_t& size) {
    auto it = values_.find(key);
    if (it == values_.end()) {
        return status::StatusCode::NoData;
    }

    size = it->second.size();

    return status::StatusCode::OK;
}

status::StatusCode MemoryStorage::read(const char* key, void* value, size_t size) {
    auto it = values_.find(key);
    if (it == values_.end()) {
        return status::StatusCode::NoData;
    }

    if (it->second.size() < size) {
        return status::StatusCode::InvalidArg;
    }

    memcpy(value, it->second.data(), std::min(size, it->second.size()));

    return status::StatusCode::OK;
}

status::StatusCode MemoryStorage::write(const char* key, const void* value, size_t size) {
    std::vector<uint8_t> data;
    data.resize(size);
    memcpy(data.data(), value, size);

    values_[key] = data;

    return status::StatusCode::OK;
}

status::StatusCode MemoryStorage::erase(const char* key) {
    return values_.erase(key) > 0 ? status::StatusCode::OK : status::StatusCode::NoData;
}

status::StatusCode MemoryStorage::erase_all() {
    if (!values_.size()) {
        return status::StatusCode::NoData;
    }

    values_.clear();

    return status::StatusCode::OK;
}

bool MemoryStorage::contains(const char* key) const {
    return values_.find(key) != values_.end();
}

} // namespace test
} // namespace ocs
