/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <string>

#include "ocs_control/fsm_block.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace control {

class FsmBlockPipeline : private core::NonCopyable<> {
public:
    struct Params {
        //! How often the FSM block should be persisted.
        system::Time state_save_interval { 0 };

        //! State interval resolution (in milliseconds, seconds, ...).
        system::Time state_interval_resolution { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p clock to count time spent in FSM states.
    //!  - @p reboot_handler to register reboot handlers.
    //!  - @p task_scheduler to register FSM tasks.
    //!  - @p storage_builder to build storage for FSM block.
    //!  - @p id to distinguish one FSM from another.
    FsmBlockPipeline(system::IClock& clock,
                     system::FanoutRebootHandler& reboot_handler,
                     scheduler::ITaskScheduler& task_scheduler,
                     storage::StorageBuilder& storage_builder,
                     const char* id,
                     Params params);

    //! Return block that maintains the FSM state.
    FsmBlock& get_block();

private:
    const std::string log_tag_;
    const std::string block_id_;
    const std::string storage_id_;

    storage::StorageBuilder::IStoragePtr storage_;

    std::unique_ptr<FsmBlock> block_;
};

} // namespace control
} // namespace ocs
