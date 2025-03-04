/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/ipath_iterator.h"
#include "ocs_http/ipath_matcher.h"
#include "ocs_http/irouter.h"
#include "ocs_http/iserver.h"

namespace ocs {
namespace http {

class Server : public IServer,
               private IPathMatcher,
               private IPathIterator,
               private core::NonCopyable<> {
public:
    struct Params {
        //! TCP port to accept incoming connections.
        unsigned server_port { 80 };

        //! Maximum allowed URI handlers.
        unsigned max_uri_handlers { 32 };
    };

    //! Initialize.
    Server(IRouter& router, Params params);

    //! Destroy.
    ~Server();

    //! Start HTTP server.
    status::StatusCode start() override;

    //! Stop HTTP server.
    status::StatusCode stop() override;

private:
    static esp_err_t handle_request_(httpd_req_t* req);

    bool match_path(const char* reference_path,
                    const char* path_to_match,
                    size_t match_upto) override;

    void iterate_path(const char* path, HandlerFunc& handler) override;

    status::StatusCode register_uris_();

    void handle_request_get_(httpd_req_t* req);

    IRouter& router_;

    httpd_handle_t handle_ { nullptr };
    httpd_config_t config_;
};

} // namespace http
} // namespace ocs
