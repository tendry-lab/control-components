/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class DynamicFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p size - underlying buffer size, in bytes, allocated on the heap.
    explicit DynamicFormatter(unsigned size);

    //! Format @p json into the underlying buffer.
    status::StatusCode format(cJSON* json) override;

    //! Return the underlying buffer.
    const char* c_str() const;

private:
    void clear_();

    const unsigned size_ { 0 };

    std::unique_ptr<char[]> buf_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
