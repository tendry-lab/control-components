/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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

    //! Return method of the underlying request.
    Method get_method() const override;

private:
    httpd_req_t& req_;
};

} // namespace http
} // namespace ocs
