/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/iheader.h"

namespace ocs {
namespace http {

class Header : public IHeader, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit Header(httpd_req_t& req);

    //! Set the header value.
    status::StatusCode set(const char* key, const char* value) override;

    //! Return true if the transfer encoding is set to chunked.
    bool chunked() const;

private:
    bool chunked_ { false };
    httpd_req_t& req_;
};

} // namespace http
} // namespace ocs
