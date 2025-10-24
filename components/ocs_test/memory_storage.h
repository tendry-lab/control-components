/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace test {

class MemoryStorage : public storage::IStorage, private core::NonCopyable<> {
public:
    status::StatusCode probe(const char* key, size_t& size) override;
    status::StatusCode read(const char* key, void* value, size_t size) override;
    status::StatusCode write(const char* key, const void* value, size_t size) override;
    status::StatusCode erase(const char* key) override;
    status::StatusCode erase_all() override;

    bool contains(const char* key) const;

private:
    std::unordered_map<std::string, std::vector<uint8_t>> values_;
};

} // namespace test
} // namespace ocs
