/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "esp_netif.h"

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_net/macros.h"
#include "ocs_net/target_esp32/ap_network.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "ap_network";

} // namespace

ApNetwork::ApNetwork(INetworkHandler& handler, const ApNetworkConfig& config)
    : config_(config)
    , handler_(handler)
    , cond_(mu_) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    netif_ = make_netif_shared(esp_netif_create_default_wifi_ap());
    configASSERT(netif_);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_event_, this, &instance_any_id_));

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
}

ApNetwork::~ApNetwork() {
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                          &instance_any_id_));

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());

    // Note: Deinitialization is not supported yet
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_netif.html#_CPPv416esp_netif_deinitv
    configASSERT(esp_netif_deinit() == ESP_ERR_NOT_SUPPORTED);
}

IApNetwork::Info ApNetwork::get_info() {
    core::LockGuard lock(mu_);

    return IApNetwork::Info {
        .channel = config_.get_channel(),
        .cur_connection = connection_count_,
        .max_connection = config_.get_max_conn(),
    };
}

status::StatusCode ApNetwork::start() {
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));

    strncpy(reinterpret_cast<char*>(wifi_config.ap.ssid), config_.get_ssid(),
            sizeof(wifi_config.ap.ssid));

    strncpy(reinterpret_cast<char*>(wifi_config.ap.password), config_.get_password(),
            sizeof(wifi_config.ap.password));

    wifi_config.ap.channel = config_.get_channel();
    wifi_config.ap.max_connection = config_.get_max_conn();
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;

    auto err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_set_config(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_start(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode ApNetwork::stop() {
    const auto err = esp_wifi_stop();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_stop(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode ApNetwork::wait(TickType_t wait) {
    core::LockGuard lock(mu_);

    while (code_ == status::StatusCode::Last) {
        if (const auto code = cond_.wait(wait); code != status::StatusCode::OK) {
            return code;
        }
    }

    return code_;
}

void ApNetwork::handle_event_(void* event_arg,
                              esp_event_base_t event_base,
                              int32_t event_id,
                              void* event_data) {
    configASSERT(event_base == WIFI_EVENT);
    configASSERT(event_arg);

    ApNetwork& self = *static_cast<ApNetwork*>(event_arg);

    if (event_base == WIFI_EVENT) {
        self.handle_wifi_event_(event_id, event_data);
    } else {
        ocs_loge(log_tag, "unsupported event: event_id=%ld", event_id);
    }
}

void ApNetwork::handle_wifi_event_(int32_t event_id, void* event_data) {
    switch (event_id) {
    case WIFI_EVENT_AP_START:
        handle_wifi_event_ap_sta_start_();
        break;

    case WIFI_EVENT_AP_STOP:
        handle_wifi_event_ap_sta_stop_();
        break;

    case WIFI_EVENT_AP_STACONNECTED:
        handle_wifi_event_ap_sta_connected_(event_data);
        break;

    case WIFI_EVENT_AP_STADISCONNECTED:
        handle_wifi_event_ap_sta_disconnected_(event_data);
        break;

    default:
        break;
    }
}

void ApNetwork::handle_wifi_event_ap_sta_start_() {
    core::LockGuard lock(mu_);
    code_ = status::StatusCode::OK;

    if (const auto code = cond_.broadcast(); code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to broadcast on START event: %s",
                 status::code_to_str(code));
    }

    handler_.handle_connect();
}

void ApNetwork::handle_wifi_event_ap_sta_stop_() {
    core::LockGuard lock(mu_);
    code_ = status::StatusCode::Error;

    if (const auto code = cond_.broadcast(); code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to broadcast on STOP event: %s",
                 status::code_to_str(code));
    }

    handler_.handle_disconnect();
}

void ApNetwork::handle_wifi_event_ap_sta_connected_(void* event_data) {
    core::LockGuard lock(mu_);
    ++connection_count_;

    wifi_event_ap_staconnected_t event =
        *static_cast<wifi_event_ap_staconnected_t*>(event_data);

    ocs_logi(log_tag, "station joined: mac=" MACSTR " AID=%d", MAC2STR(event.mac),
             event.aid);
}

void ApNetwork::handle_wifi_event_ap_sta_disconnected_(void* event_data) {
    core::LockGuard lock(mu_);
    --connection_count_;

    wifi_event_ap_staconnected_t event =
        *static_cast<wifi_event_ap_staconnected_t*>(event_data);

    ocs_logi(log_tag, "station left: mac=" MACSTR " AID=%d", MAC2STR(event.mac),
             event.aid);
}

} // namespace net
} // namespace ocs
