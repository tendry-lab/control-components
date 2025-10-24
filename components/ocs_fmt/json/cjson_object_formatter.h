/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

#include "cJSON.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace fmt {
namespace json {

class CjsonObjectFormatter : private core::NonCopyable<> {
public:
    //! Initialize.
    explicit CjsonObjectFormatter(cJSON* json);

    //! Add constant string @p value with constant @p key to @p json.
    bool add_string_ref_cs(const char* key, const char* value);

    //! Add @p value with constant @p key to @p json.
    bool add_string_cs(const char* key, const char* value);

    //! Add @p value with constant @p key to @p json.
    bool add_number_cs(const char* key, double value);

    //! Add boolean @p value with constant @p key to @p json.
    bool add_bool_cs(const char* key, bool value);

    //! Add True with constant @p key to @p json.
    bool add_true_cs(const char* key);

    //! Add False with constant @p key to @p json.
    bool add_false_cs(const char* key);

    //! Add Null with constant @p key to @p json.
    bool add_null_cs(const char* key);

private:
    cJSON* json_ { nullptr };
};

} // namespace json
} // namespace fmt
} // namespace ocs
