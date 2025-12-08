/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_storage/istorage.h"

namespace ocs {
namespace algo {

struct StorageOps {
    //! Read string with @p key from @p storage to @p buf which can hold @p size bytes.
    static status::StatusCode
    prob_read(storage::IStorage& storage, const char* key, void* buf, size_t size);
};

} // namespace algo
} // namespace ocs
