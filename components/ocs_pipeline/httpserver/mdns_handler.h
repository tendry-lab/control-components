/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_http/irouter.h"
#include "ocs_net/mdns_config.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class MdnsHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register HTTP endpoints.
    //!  - @p config to perform the mDNS configuration.
    //!  - @p reboot_task to schedule a reboot when the mDNS configuration is changed.
    MdnsHandler(http::IRouter& router,
                net::MdnsConfig& config,
                scheduler::ITask& reboot_task);

private:
    status::StatusCode handle_update_(http::IResponseWriter&,
                                      const algo::UriOps::Values&);

    core::StaticMutex mu_;
    net::MdnsConfig& config_;
    scheduler::ITask& reboot_task_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
