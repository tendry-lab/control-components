/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_algo/mdns_ops.h"

namespace ocs {
namespace algo {

void MdnsOps::enable_autodiscovery(net::MdnsService& service, const char* path) {
    auto uri = std::string(service.get_scheme()) + "://" + service.get_hostname() + "."
        + service.get_domain() + ":" + std::to_string(service.get_port());
    if (path) {
        uri += path;
    }

    service.add_txt_record("autodiscovery_uri", uri.c_str());
    service.add_txt_record("autodiscovery_desc", service.get_instance_name());
    service.add_txt_record("autodiscovery_mode", "1");
}

} // namespace algo
} // namespace ocs
