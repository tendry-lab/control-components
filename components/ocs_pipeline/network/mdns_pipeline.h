/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"
#include "ocs_system/fanout_suspender.h"
#include "ocs_system/isuspend_handler.h"

namespace ocs {
namespace pipeline {
namespace network {

class MdnsPipeline : private system::ISuspendHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    MdnsPipeline(system::FanoutSuspender& suspender,
                 const char* hostname,
                 const char* instance_name);

    //! Start the pipeline.
    status::StatusCode start();

    //! Get the underlying mDNS driver.
    net::IMdnsDriver& get_driver();

private:
    status::StatusCode handle_suspend() override;
    status::StatusCode handle_resume() override;

    std::unique_ptr<net::IMdnsDriver> driver_;
    std::unique_ptr<net::IMdnsDriver> store_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
