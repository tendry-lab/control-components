/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/iserver.h"

namespace ocs {
namespace http {

class Server : public IServer, public core::NonCopyable<> {
public:
    struct Params {
        //! TCP port to accept incoming connections.
        unsigned server_port { 80 };

        //! Maximum allowed URI handlers.
        unsigned max_uri_handlers { 32 };
    };

    //! Initialize.
    explicit Server(const Params& params);

    //! Destroy.
    ~Server();

    //! Start HTTP server.
    status::StatusCode start() override;

    //! Stop HTTP server.
    status::StatusCode stop() override;

    //! Register HTTP handler for a GET request.
    void add_GET(const char* path, HandlerFunc func) override;

private:
    using Endpoint = std::pair<std::string, HandlerFunc>;
    using EndpointList = std::vector<Endpoint>;

    static esp_err_t handle_request_(httpd_req_t* req);

    status::StatusCode register_uris_();

    void handle_request_get_(httpd_req_t* req);

    httpd_handle_t handle_ { nullptr };
    httpd_config_t config_;

    EndpointList endpoints_get_;
};

} // namespace http
} // namespace ocs
