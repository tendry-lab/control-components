/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/basic/system_pipeline.h"
#include "ocs_scheduler/async_func.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/delay_rebooter.h"
#include "ocs_system/reboot_task.h"
#include "ocs_system/target_esp32/default_clock.h"

namespace ocs {
namespace pipeline {
namespace basic {

SystemPipeline::SystemPipeline(SystemPipeline::Params params) {
    configASSERT(params.task_scheduler.delay);

    flash_initializer_.reset(new (std::nothrow) storage::FlashInitializer());
    configASSERT(flash_initializer_);

    storage_builder_.reset(new (std::nothrow) storage::StorageBuilder());
    configASSERT(storage_builder_);

    default_clock_.reset(new (std::nothrow) system::DefaultClock());
    configASSERT(default_clock_);

    delay_estimator_.reset(new (std::nothrow) scheduler::ConstantDelayEstimator(
        params.task_scheduler.delay));
    configASSERT(delay_estimator_);

    task_scheduler_.reset(new (std::nothrow) scheduler::PeriodicTaskScheduler(
        *default_clock_, *delay_estimator_, "system_pipeline_scheduler", 16));
    configASSERT(task_scheduler_);

    func_scheduler_.reset(new (std::nothrow) scheduler::AsyncFuncScheduler(16));
    configASSERT(func_scheduler_);

    configASSERT(task_scheduler_->add(*func_scheduler_, "system_async_func_scheduler",
                                      system::Duration::millisecond * 500)
                 == status::StatusCode::OK);

    fanout_reboot_handler_.reset(new (std::nothrow) system::FanoutRebootHandler());
    configASSERT(fanout_reboot_handler_);

    default_rebooter_ = system::PlatformBuilder::make_rebooter(*fanout_reboot_handler_);
    configASSERT(default_rebooter_);

    rebooter_ = default_rebooter_.get();

    delay_rebooter_.reset(new (std::nothrow)
                              system::DelayRebooter(pdMS_TO_TICKS(500), *rebooter_));
    configASSERT(delay_rebooter_);

    rebooter_ = delay_rebooter_.get();

    reboot_task_.reset(new (std::nothrow) system::RebootTask(*rebooter_));
    configASSERT(reboot_task_);

    reboot_task_async_.reset(new (std::nothrow)
                                 scheduler::AsyncFunc(*func_scheduler_, *reboot_task_));
    configASSERT(reboot_task_async_);

    device_info_.reset(new (std::nothrow) system::DeviceInfo(
        CONFIG_OCS_CORE_FW_NAME, CONFIG_OCS_CORE_FW_VERSION,
        CONFIG_OCS_CORE_FW_DESCRIPTION, CONFIG_OCS_CORE_PRODUCT_NAME));
    configASSERT(device_info_);

    fsr_handler_.reset(new (std::nothrow) control::ConfigFsrHandler());
    configASSERT(fsr_handler_);
}

status::StatusCode SystemPipeline::start() {
    configASSERT(task_scheduler_->start() == status::StatusCode::OK);

    while (true) {
        configASSERT(task_scheduler_->run() == status::StatusCode::OK);
    }

    return status::StatusCode::OK;
}

system::IClock& SystemPipeline::get_clock() {
    return *default_clock_;
}

storage::StorageBuilder& SystemPipeline::get_storage_builder() {
    return *storage_builder_;
}

scheduler::AsyncFuncScheduler& SystemPipeline::get_func_scheduler() {
    return *func_scheduler_;
}

scheduler::ITaskScheduler& SystemPipeline::get_task_scheduler() {
    return *task_scheduler_;
}

scheduler::ITask& SystemPipeline::get_reboot_task() {
    return *reboot_task_async_;
}

system::IRebooter& SystemPipeline::get_rebooter() {
    return *rebooter_;
}

system::FanoutRebootHandler& SystemPipeline::get_reboot_handler() {
    return *fanout_reboot_handler_;
}

const system::DeviceInfo& SystemPipeline::get_device_info() const {
    return *device_info_;
}

control::ConfigFsrHandler& SystemPipeline::get_fsr_handler() {
    return *fsr_handler_;
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
