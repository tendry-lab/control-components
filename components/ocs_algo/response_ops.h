/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_http/iresponse_writer.h"
#include "ocs_status/code.h"

namespace ocs {
namespace algo {

struct ResponseOps {
    //! Write text to HTTP response.
    static status::StatusCode write_text(http::IResponseWriter& w, const char* text);

    //! Write JSON to HTTP response.
    static status::StatusCode write_json(http::IResponseWriter& w, const char* buf);
};

} // namespace algo
} // namespace ocs
