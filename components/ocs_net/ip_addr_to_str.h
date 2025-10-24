/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "lwip/ip_addr.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace net {

class ip_addr_to_str : private core::NonCopyable<> {
public:
    //! Initialize.
    explicit ip_addr_to_str(ip_addr_t addr);

    //! Return formatted address.
    const char* c_str() const;

private:
    char buf_[IPADDR_STRLEN_MAX];
};

} // namespace net
} // namespace ocs
