/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_algo/sha_engine_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class sha_to_hex_str : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p sha - SHA data, @p size bytes long.
    sha_to_hex_str(const uint8_t* sha, size_t size);

    //! Return SHA formatted as a hex string.
    const char* c_str() const;

private:
    static constexpr size_t max_hash_length_ =
        algo::ShaEngineOps::hash_lenght(IShaEngine::Algorithm::SHA512);

    char buf_[max_hash_length_ + 1];
};

} // namespace security
} // namespace ocs
