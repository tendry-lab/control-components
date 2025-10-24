/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_net/mdns_to_str.h"

namespace ocs {
namespace net {

const char* mdns_service_type_to_str(MdnsService::ServiceType service_type) {
    switch (service_type) {
    case MdnsService::ServiceType::Http:
        return "_http";

    default:
        break;
    }

    return "<none>";
}

const char* mdns_proto_to_str(MdnsService::Proto proto) {
    switch (proto) {
    case MdnsService::Proto::Tcp:
        return "_tcp";

    default:
        break;
    }

    return "<none>";
}

} // namespace net
} // namespace ocs
