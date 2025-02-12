/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "mdns.h"

#include "ocs_core/log.h"
#include "ocs_net/default_mdns_server.h"
#include "ocs_net/mdns_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "default_mdns_server";

} // namespace

DefaultMdnsServer::DefaultMdnsServer(const char* hostname)
    : BasicMdnsServer(hostname) {
}

status::StatusCode DefaultMdnsServer::start() {
    auto err = mdns_init();
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_init(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = mdns_hostname_set(hostname_.c_str());
    if (err != ESP_OK) {
        ocs_loge(log_tag, "mdns_hostname_set(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    for (auto& service : services_) {
        configASSERT(hostname_ == service->get_hostname());

        err = mdns_service_add_for_host(service->get_instance_name(),
                                        service->get_service_type(), service->get_proto(),
                                        service->get_hostname(), service->get_port(),
                                        nullptr, 0);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "mdns_service_add_for_host(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }

        for (const auto& [key, value] : service->get_txt_records()) {
            err = mdns_service_txt_item_set_for_host(
                service->get_instance_name(), service->get_service_type(),
                service->get_proto(), service->get_hostname(), key.c_str(),
                value.c_str());
            if (err != ESP_OK) {
                ocs_loge(log_tag, "mdns_service_txt_item_set_for_host(): %s",
                         esp_err_to_name(err));

                return status::StatusCode::Error;
            }
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode DefaultMdnsServer::stop() {
    mdns_free();
    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
