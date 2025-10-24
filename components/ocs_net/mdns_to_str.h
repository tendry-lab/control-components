/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_net/mdns_service.h"

namespace ocs {
namespace net {

//! Return string representation of the mDNS service type.
const char* mdns_service_type_to_str(MdnsService::ServiceType service_type);

//! Return string representation of the mDNS transport protocol.
const char* mdns_proto_to_str(MdnsService::Proto proto);

} // namespace net
} // namespace ocs
