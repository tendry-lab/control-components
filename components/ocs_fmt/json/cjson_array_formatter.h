/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "cJSON.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace fmt {
namespace json {

class CjsonArrayFormatter : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  - @p json should be an array.
    explicit CjsonArrayFormatter(cJSON* json);

    //! Append 16-bit number stored to json.
    bool append_uint16(uint16_t value);

    //! Append a string to json.
    bool append_string(const char* str);

    //! Append a constant string to json.
    bool append_string_ref(const char* str);

private:
    cJSON* json_ { nullptr };
};

} // namespace json
} // namespace fmt
} // namespace ocs
