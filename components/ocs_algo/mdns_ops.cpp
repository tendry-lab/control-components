/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_algo/mdns_ops.h"

namespace ocs {
namespace algo {

void MdnsOps::enable_autodiscovery(net::MdnsService& service,
                                   const char* type,
                                   const char* path) {
    auto uri = std::string(service.get_scheme()) + "://" + service.get_hostname() + "."
        + service.get_domain() + ":" + std::to_string(service.get_port());
    if (path) {
        uri += path;
    }

    service.add_txt_record("autodiscovery_uri", uri.c_str());
    service.add_txt_record("autodiscovery_type", type);
    service.add_txt_record("autodiscovery_desc", service.get_instance_name());
    service.add_txt_record("autodiscovery_mode", "1");
}

} // namespace algo
} // namespace ocs
