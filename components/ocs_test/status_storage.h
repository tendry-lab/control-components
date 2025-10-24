/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace test {

class StatusStorage : public storage::IStorage, private core::NonCopyable<> {
public:
    explicit StatusStorage(storage::IStorage& storage);

    status::StatusCode probe(const char* key, size_t& size) override;
    status::StatusCode read(const char* key, void* value, size_t size) override;
    status::StatusCode write(const char* key, const void* value, size_t size) override;
    status::StatusCode erase(const char* key) override;
    status::StatusCode erase_all() override;

    status::StatusCode probe_status { status::StatusCode::OK };
    status::StatusCode read_status { status::StatusCode::OK };
    status::StatusCode write_status { status::StatusCode::OK };
    status::StatusCode erase_status { status::StatusCode::OK };
    status::StatusCode erase_all_status { status::StatusCode::OK };

private:
    storage::IStorage& storage_;
};

} // namespace test
} // namespace ocs
