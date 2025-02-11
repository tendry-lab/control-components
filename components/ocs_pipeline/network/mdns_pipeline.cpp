/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_net/default_mdns_driver.h"
#include "ocs_net/mdns_store.h"
#include "ocs_pipeline/network/mdns_pipeline.h"

namespace ocs {
namespace pipeline {
namespace network {

MdnsPipeline::MdnsPipeline(system::FanoutSuspender& suspender,
                           config::MdnsConfig& config) {
    configASSERT(suspender.add(*this, "mdns_pipeline") == status::StatusCode::OK);

    driver_.reset(new (std::nothrow) net::DefaultMdnsDriver(config.get_hostname(),
                                                            config.get_instance_name()));
    configASSERT(driver_);

    store_.reset(new (std::nothrow) net::MdnsStore(*driver_));
    configASSERT(store_);
}

status::StatusCode MdnsPipeline::start() {
    return store_->start();
}

net::IMdnsDriver& MdnsPipeline::get_driver() {
    return *store_;
}

status::StatusCode MdnsPipeline::handle_suspend() {
    return store_->stop();
}

status::StatusCode MdnsPipeline::handle_resume() {
    return start();
}

} // namespace network
} // namespace pipeline
} // namespace ocs
