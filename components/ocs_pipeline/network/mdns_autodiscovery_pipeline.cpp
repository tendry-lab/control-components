/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/network/mdns_autodiscovery_pipeline.h"

namespace ocs {
namespace pipeline {
namespace network {

MdnsAutodiscoveryPipeline::MdnsAutodiscoveryPipeline(net::IMdnsDriver& driver,
                                                     config::MdnsConfig& config)
    : driver_(driver)
    , config_(config) {
}

status::StatusCode MdnsAutodiscoveryPipeline::add(net::IMdnsDriver::Service service,
                                                  unsigned port,
                                                  const char* path) {
    const char* scheme = nullptr;

    switch (service) {
    case net::IMdnsDriver::Service::Http:
        scheme = "http";
    }

    auto uri =
        std::string(scheme) + "://" + driver_.get_dns_name() + ":" + std::to_string(port);
    if (path) {
        uri += path;
    }

    auto code = driver_.add_txt_record(net::IMdnsDriver::Service::Http,
                                       net::IMdnsDriver::Proto::Tcp, "autodiscovery_uri",
                                       uri.c_str());
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = driver_.add_txt_record(net::IMdnsDriver::Service::Http,
                                  net::IMdnsDriver::Proto::Tcp, "autodiscovery_desc",
                                  config_.get_instance_name());
    if (code != status::StatusCode::OK) {
        return code;
    }

    code =
        driver_.add_txt_record(net::IMdnsDriver::Service::Http,
                               net::IMdnsDriver::Proto::Tcp, "autodiscovery_mode", "1");
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace network
} // namespace pipeline
} // namespace ocs
