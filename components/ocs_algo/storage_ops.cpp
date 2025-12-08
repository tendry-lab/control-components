/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace algo {

namespace {

const char* log_tag = "storage_ops";

} // namespace

status::StatusCode StorageOps::prob_read(storage::IStorage& storage,
                                         const char* key,
                                         void* buf,
                                         size_t size) {
    size_t recv_size = 0;
    auto code = storage.probe(key, recv_size);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to prob object size from storage: key=%s code=%s",
                     key, status::code_to_str(code));
        }

        return code;
    }

    if (recv_size > size) {
        ocs_loge(log_tag,
                 "failed to read object from storage: invalid size: max=%u got=%u", size,
                 recv_size);

        return status::StatusCode::InvalidArg;
    }

    code = storage.read(key, buf, recv_size);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to read object from storage: key=%s code=%s", key,
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

} // namespace algo
} // namespace ocs
