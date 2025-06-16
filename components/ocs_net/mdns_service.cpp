/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/mdns_service.h"
#include "ocs_net/mdns_to_str.h"

namespace ocs {
namespace net {

MdnsService::MdnsService(const char* instance_name,
                         ServiceType service_type,
                         Proto proto,
                         const char* domain,
                         const char* hostname,
                         uint16_t port)
    : instance_name_(instance_name)
    , service_type_(service_type)
    , proto_(proto)
    , domain_(domain)
    , hostname_(hostname)
    , port_(port) {
}

const char* MdnsService::get_instance_name() const {
    return instance_name_.c_str();
}

const char* MdnsService::get_service_type() const {
    return mdns_service_type_to_str(service_type_);
}

const char* MdnsService::get_proto() const {
    return mdns_proto_to_str(proto_);
}

const char* MdnsService::get_domain() const {
    return domain_.c_str();
}

const char* MdnsService::get_hostname() const {
    return hostname_.c_str();
}

uint16_t MdnsService::get_port() const {
    return port_;
}

const MdnsService::TxtRecordList& MdnsService::get_txt_records() const {
    return txt_records_;
}

const char* MdnsService::get_scheme() const {
    switch (service_type_) {
    case ServiceType::Http:
        return "http";
    }

    return "<none>";
}

void MdnsService::add_txt_record(const char* key, const char* value) {
    txt_records_.push_back(std::make_pair(key, value));
}

} // namespace net
} // namespace ocs
