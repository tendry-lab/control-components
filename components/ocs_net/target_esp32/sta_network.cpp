/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "esp_netif.h"

#include "ocs_core/log.h"
#include "ocs_net/target_esp32/sta_network.h"

#define EVENT_BIT_CONNECTED BIT0
#define EVENT_BIT_FAILED BIT1

namespace ocs {
namespace net {

namespace {

const char* log_tag = "sta_network";

int8_t get_rssi() {
    wifi_ap_record_t record;
    memset(&record, 0, sizeof(record));

    const auto err = esp_wifi_sta_get_ap_info(&record);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_sta_get_ap_info(): %s", esp_err_to_name(err));
        return 0;
    }

    return record.rssi;
}

} // namespace

StaNetwork::StaNetwork(INetworkHandler& handler, const StaNetworkConfig& config)
    : config_(config)
    , handler_(handler) {
    configASSERT(config_.valid());

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    netif_ = make_netif_shared(esp_netif_create_default_wifi_sta());
    configASSERT(netif_);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_event_, this, &instance_any_id_));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &handle_event_, this, &instance_got_ip_));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
}

StaNetwork::~StaNetwork() {
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                                          &instance_any_id_));

    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                                          &instance_got_ip_));

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());

    // Note: Deinitialization is not supported yet
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/network/esp_netif.html#_CPPv416esp_netif_deinitv
    configASSERT(esp_netif_deinit() == ESP_ERR_NOT_SUPPORTED);
}

IStaNetwork::Info StaNetwork::get_info() {
    return IStaNetwork::Info {
        .ssid = config_.get_ssid(),
        .rssi = get_rssi(),
        .ip_addr = get_ip_addr_(),
    };
}

status::StatusCode StaNetwork::start() {
    wifi_config_t wifi_config;
    memset(&wifi_config, 0, sizeof(wifi_config));

    strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), config_.get_ssid(),
            sizeof(wifi_config.sta.ssid));

    strncpy(reinterpret_cast<char*>(wifi_config.sta.password), config_.get_password(),
            sizeof(wifi_config.sta.password));

    auto err = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
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

status::StatusCode StaNetwork::stop() {
    const auto err = esp_wifi_stop();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_stop(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode StaNetwork::wait(TickType_t wait) {
    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), EVENT_BIT_CONNECTED | EVENT_BIT_FAILED,
                            pdFALSE, pdFALSE, wait);

    if (bits & EVENT_BIT_CONNECTED) {
        ocs_logi(log_tag, "connected to AP: SSID=%s RSSI=%d", config_.get_ssid(),
                 get_rssi());

        return status::StatusCode::OK;
    }

    if (bits & EVENT_BIT_FAILED) {
        ocs_logi(log_tag, "failed to connect to AP: SSID=%s", config_.get_ssid());

        return status::StatusCode::Error;
    }

    ocs_loge(log_tag, "unexpected event bit: bits=%ld", bits);
    configASSERT(false);
}

void StaNetwork::handle_event_(void* event_arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void* event_data) {
    configASSERT(event_base == WIFI_EVENT || event_base == IP_EVENT);
    configASSERT(event_arg);

    StaNetwork& self = *static_cast<StaNetwork*>(event_arg);

    if (event_base == WIFI_EVENT) {
        self.handle_wifi_event_(event_id, event_data);
    } else if (event_base == IP_EVENT) {
        self.handle_ip_event_(event_id, event_data);
    } else {
        ocs_loge(log_tag, "unsupported event: event_id=%ld", event_id);
    }
}

ip_addr_t StaNetwork::get_ip_addr_() const {
    const EventBits_t bits = xEventGroupGetBits(event_group_.get());
    if (!(bits & EVENT_BIT_CONNECTED)) {
        return IPADDR4_INIT(0);
    }

    esp_netif_ip_info_t ip_info;
    memset(&ip_info, 0, sizeof(ip_info));

    const auto err = esp_netif_get_ip_info(netif_.get(), &ip_info);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_netif_get_ip_info(): %s", esp_err_to_name(err));
        return IPADDR4_INIT(0);
    }

    return IPADDR4_INIT(ip_info.ip.addr);
}

void StaNetwork::handle_wifi_event_(int32_t event_id, void* event_data) {
    switch (event_id) {
    case WIFI_EVENT_STA_START:
        handle_wifi_event_sta_start_();
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        handle_wifi_event_sta_disconnected_(event_data);
        break;

    default:
        break;
    }
}

void StaNetwork::handle_wifi_event_sta_start_() {
    ocs_logi(log_tag, "WIFI_EVENT_STA_START");

    const esp_err_t err = esp_wifi_connect();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_wifi_connect(): %s", esp_err_to_name(err));
    }
}

void StaNetwork::handle_wifi_event_sta_disconnected_(void* event_data) {
    configASSERT(event_data);

    wifi_event_sta_disconnected_t& data =
        *static_cast<wifi_event_sta_disconnected_t*>(event_data);

    ocs_logi(log_tag, "WIFI_EVENT_STA_DISCONNECTED: reason=%u", data.reason);

    xEventGroupClearBits(event_group_.get(), EVENT_BIT_CONNECTED);

    if (retry_count_ < config_.get_max_retry_count()) {
        ++retry_count_;

        const esp_err_t err = esp_wifi_connect();
        if (err != ESP_OK) {
            ocs_loge(log_tag, "esp_wifi_connect(): %s", esp_err_to_name(err));
        } else {
            ocs_logi(log_tag, "reconnecting: retry_count=%u max_retry_count=%u",
                     retry_count_, config_.get_max_retry_count());
        }
    } else {
        xEventGroupSetBits(event_group_.get(), EVENT_BIT_FAILED);
    }

    handler_.handle_disconnect();
}

void StaNetwork::handle_ip_event_(int32_t event_id, void* event_data) {
    switch (event_id) {
    case IP_EVENT_STA_GOT_IP:
        handle_ip_event_sta_got_ip_(event_data);
        break;

    default:
        break;
    }
}

void StaNetwork::handle_ip_event_sta_got_ip_(void* event_data) {
    ip_event_got_ip_t& event = *static_cast<ip_event_got_ip_t*>(event_data);
    ocs_logi(log_tag, "got ip:" IPSTR "", IP2STR(&event.ip_info.ip));

    retry_count_ = 0;
    xEventGroupSetBits(event_group_.get(), EVENT_BIT_CONNECTED);

    handler_.handle_connect();
}

} // namespace net
} // namespace ocs
