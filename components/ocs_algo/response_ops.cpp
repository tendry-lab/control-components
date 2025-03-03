/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace algo {

status::StatusCode ResponseOps::write_text(http::IResponseWriter& w, const char* text) {
    OCS_STATUS_RETURN_ON_ERROR(
        w.get_header().set("Content-Type", "text/plain; charset=utf-8"));

    OCS_STATUS_RETURN_ON_ERROR(
        w.get_header().set("Content-Length", std::to_string(strlen(text)).c_str()));

    OCS_STATUS_RETURN_ON_ERROR(w.write_status(http::StatusCode::OK));

    return w.write(text, strlen(text));
}

status::StatusCode ResponseOps::write_json(http::IResponseWriter& w, const char* buf) {
    OCS_STATUS_RETURN_ON_ERROR(w.get_header().set("Content-Type", "application/json"));

    OCS_STATUS_RETURN_ON_ERROR(
        w.get_header().set("Content-Length", std::to_string(strlen(buf)).c_str()));

    OCS_STATUS_RETURN_ON_ERROR(w.write_status(http::StatusCode::OK));

    return w.write(buf, strlen(buf));
}

} // namespace algo
} // namespace ocs
