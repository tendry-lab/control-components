/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstring>

#include "ocs_core/noncopyable.h"
#include "ocs_core/version.h"

namespace ocs {
namespace core {

class version_to_str : private NonCopyable<> {
public:
    //! Initialize.
    explicit version_to_str(Version version);

    //! Return formatted version.
    const char* c_str() const;

private:
    static constexpr unsigned size_ = strlen("65535.65535.65535");

    char buf_[size_ + 1];
};

} // namespace core
} // namespace ocs
