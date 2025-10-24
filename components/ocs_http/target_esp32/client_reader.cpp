/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_http/target_esp32/client_reader.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace http {

namespace {

const char* log_tag = "http_client_reader";

} // namespace

ClientReader::ClientReader(const Params& params)
    : params_(params)
    , cond_(mu_) {
    memset(&config_, 0, sizeof(config_));
    config_.host = params_.host.c_str();
    config_.path = params_.path.c_str();
    config_.transport_type = HTTP_TRANSPORT_OVER_TCP;
    config_.event_handler = handle_event_;
    config_.user_data = this;

    client_ = make_client_shared(config_);
}

esp_http_client_handle_t ClientReader::client() const {
    return client_.get();
}

status::StatusCode ClientReader::wait(TickType_t wait) {
    core::LockGuard lock(mu_);

    while (!data_received_) {
        OCS_STATUS_RETURN_ON_ERROR(cond_.wait(wait));
    }

    return status::StatusCode::OK;
}

unsigned ClientReader::read(char* buf, unsigned size) {
    core::LockGuard lock(mu_);

    const unsigned len = std::min(size, buf_.size());
    memcpy(buf, buf_.data(), len);

    return len;
}

esp_err_t ClientReader::handle_event_(esp_http_client_event_t* event) {
    configASSERT(event->user_data);
    ClientReader& self = *static_cast<ClientReader*>(event->user_data);

    switch (event->event_id) {
    case HTTP_EVENT_ERROR:
        ocs_logd(log_tag, "HTTP_EVENT_ERROR");
        break;

    case HTTP_EVENT_ON_CONNECTED:
        ocs_logd(log_tag, "HTTP_EVENT_ON_CONNECTED");
        break;

    case HTTP_EVENT_HEADER_SENT:
        ocs_logd(log_tag, "HTTP_EVENT_HEADER_SENT");
        break;

    case HTTP_EVENT_ON_HEADER:
        ocs_logd(log_tag, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", event->header_key,
                 event->header_value);
        break;

    case HTTP_EVENT_ON_DATA:
        ocs_logd(log_tag, "HTTP_EVENT_ON_DATA, len=%d", event->data_len);
        self.handle_event_on_data_(event);
        break;

    case HTTP_EVENT_ON_FINISH:
        ocs_logd(log_tag, "HTTP_EVENT_ON_FINISH");
        self.handle_event_on_finish_();
        break;

    case HTTP_EVENT_DISCONNECTED:
        ocs_logi(log_tag, "HTTP_EVENT_DISCONNECTED");
        break;

    case HTTP_EVENT_REDIRECT:
        ocs_logd(log_tag, "HTTP_EVENT_REDIRECT");
        break;
    }

    return ESP_OK;
}

void ClientReader::handle_event_on_data_(esp_http_client_event_t* event) {
    core::LockGuard lock(mu_);

    const char* ptr = static_cast<const char*>(event->data);

    for (unsigned n = 0; n < event->data_len; ++n) {
        buf_.push_back(ptr[n]);
    }
}

void ClientReader::handle_event_on_finish_() {
    core::LockGuard lock(mu_);

    data_received_ = true;

    cond_.broadcast();
}

} // namespace http
} // namespace ocs
