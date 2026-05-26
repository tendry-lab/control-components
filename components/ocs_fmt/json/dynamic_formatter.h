/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace fmt {
namespace json {

class DynamicFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p size - underlying buffer size, in bytes, allocated on the heap.
    DynamicFormatter(system::IArena& arena, size_t size);

    //! Format @p json into the underlying buffer.
    status::StatusCode format(cJSON* json) override;

    //! Return the underlying buffer.
    const char* c_str() const;

private:
    void clear_();

    const size_t size_ { 0 };

    system::UniquePtr<char[]> buf_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
