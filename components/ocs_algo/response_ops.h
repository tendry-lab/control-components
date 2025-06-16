/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
