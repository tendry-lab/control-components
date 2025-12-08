/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class ShaGenerator : private core::NonCopyable<> {
public:
    using Data = std::vector<uint8_t>;

    //! Initialize.
    explicit ShaGenerator(IShaEngine& engine);

    //! Return the result of SHA calculation.
    const Data& get_sha() const;

    //! Add @p size bytes from @p buf to be used during SHA calculation.
    void add(const uint8_t* buf, size_t size);

    //! Generate SHA based on the underlying data.
    status::StatusCode generate();

private:
    IShaEngine& engine_;

    Data src_;
    Data sha_;
};

} // namespace security
} // namespace ocs
