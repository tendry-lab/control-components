/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_http/target_esp32/header.h"

namespace ocs {
namespace http {

class ResponseWriter : public IResponseWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p req to send HTTP responses.
    explicit ResponseWriter(httpd_req_t& req);

    //! Write @p size bytes of @p data to the underlying request.
    status::StatusCode write(const void* data, unsigned size) override;

    //! Write HTTP status code.
    status::StatusCode write_status(StatusCode) override;

    //! Return the underlying header.
    IHeader& get_header() override;

private:
    httpd_req_t& req_;
    Header header_;
};

} // namespace http
} // namespace ocs
