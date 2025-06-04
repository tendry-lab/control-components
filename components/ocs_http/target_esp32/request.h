/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/irequest.h"

namespace ocs {
namespace http {

class Request : public IRequest, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit Request(httpd_req_t& req);

    //! Return URI of the underlying request.
    const char* get_uri() const override;

private:
    httpd_req_t& req_;
};

} // namespace http
} // namespace ocs
