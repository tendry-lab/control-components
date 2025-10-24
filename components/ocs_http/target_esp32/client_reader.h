/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "esp_http_client.h"

#include "ocs_core/cond.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/target_esp32/client_builder.h"

namespace ocs {
namespace http {

class ClientReader : private core::NonCopyable<> {
public:
    struct Params {
        //! Domain or IP as string.
        std::string host;

        //! HTTP Path, if not set, default is `/`.
        std::string path;
    };

    //! Initialize.
    explicit ClientReader(const Params& params);

    //! Return the underlying HTTP client handle.
    esp_http_client_handle_t client() const;

    //! Wait for the response.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY);

    //! Read the response.
    //!
    //! @return
    //!  Number of bytes read.
    unsigned read(char* buf, unsigned size);

private:
    static esp_err_t handle_event_(esp_http_client_event_t* event);

    void handle_event_on_data_(esp_http_client_event_t* event);
    void handle_event_on_finish_();

    const Params params_;

    esp_http_client_config_t config_;
    ClientSharedPtr client_;

    core::StaticMutex mu_;
    core::Cond cond_;

    bool data_received_ { false };
    std::vector<char> buf_;
};

} // namespace http
} // namespace ocs
