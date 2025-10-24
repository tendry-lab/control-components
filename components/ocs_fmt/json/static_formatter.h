/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstring>
#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/ijson_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

template <unsigned Size>
class StaticFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    StaticFormatter() {
        clear_();
    }

    //! Format @p json to the underlying buffer.
    status::StatusCode format(cJSON* json) override {
        clear_();

        if (!cJSON_PrintPreallocated(json, buf_, sizeof(buf_), false)) {
            return status::StatusCode::NoMem;
        }

        return status::StatusCode::OK;
    }

    //! Return the underlying buffer.
    const char* c_str() const {
        return buf_;
    }

private:
    void clear_() {
        memset(buf_, 0, sizeof(buf_));
    }

    char buf_[Size];
};

} // namespace json
} // namespace fmt
} // namespace ocs
