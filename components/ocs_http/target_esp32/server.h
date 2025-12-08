/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_http_server.h"

#include "ocs_core/noncopyable.h"
#include "ocs_http/ipattern_iterator.h"
#include "ocs_http/ipattern_matcher.h"
#include "ocs_http/irouter.h"
#include "ocs_http/iserver.h"

namespace ocs {
namespace http {

class Server : public IServer,
               private IPatternMatcher,
               private IPatternIterator,
               private core::NonCopyable<> {
public:
    struct Params {
        //! TCP port to accept incoming connections.
        uint8_t server_port { 80 };

        //! Maximum allowed URI handlers.
        uint8_t max_uri_handlers { 32 };
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
    static esp_err_t handle_request_cb_(httpd_req_t* req);

    bool match_pattern(const char* reference_pattern,
                       const char* pattern_to_match,
                       size_t match_upto) override;

    void iterate_pattern(IRequest::Method method,
                         const char* path,
                         IHandler& handler) override;

    status::StatusCode register_uris_();

    void handle_request_(httpd_req_t* req);

    IRouter& router_;

    httpd_handle_t handle_ { nullptr };
    httpd_config_t config_;
};

} // namespace http
} // namespace ocs
