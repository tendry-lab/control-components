/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/iclock.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace control {

//! FSM state.
class FsmBlock : public system::IRebootHandler,
                 public scheduler::ITask,
                 private core::NonCopyable<> {
public:
    using State = uint16_t;

    //! Initialize.
    //!
    //! @params
    //!  - @p clock for counting the duration of states.
    //!  - @p storage to persist the block state.
    //!  - @p resolution - state interval resolution (in milliseconds, seconds, ...).
    //!  - @p id to distinguish one block from another.
    FsmBlock(system::IClock& clock,
             storage::IStorage& storage,
             system::Time resolution,
             const char* id);

    //! Save block state to the storage.
    //!
    //! @remarks
    //!  All operations should be scheduled on the same task scheduler.
    void handle_reboot() override;

    //! Save block state in the storage.
    //!
    //! @remarks
    //!  All operations should be scheduled on the same task scheduler.
    status::StatusCode run() override;

    //! Return the previous FSM state.
    State previous_state() const;

    //! Return the current FSM state.
    State current_state() const;

    //! Return the next FSM state.
    State next_state() const;

    //! Return the previous FSM state duration.
    system::Time previous_state_duration() const;

    //! Return the current FSM state duration.
    system::Time current_state_duration() const;

    //! Return the number of times the block was persisted in the storage.
    //!
    //! @remarks
    //!  Very frequent writes can damage the underlying storage.
    uint64_t write_count() const;

    //! Return true if the block is ready to transit to the next state.
    bool is_in_transit() const;

    //! Transit block to the next state.
    status::StatusCode transit();

    //! Update time, the block spent in the current state.
    void update();

    //! Set the next FSM state.
    void set_next(State state);

private:
    status::StatusCode read_();
    status::StatusCode write_();

    const std::string log_tag_;
    const system::Time resolution_ { 0 };

    system::IClock& clock_;
    storage::IStorage& storage_;

    //! Previous machine state.
    State prev_state_ { 0 };

    //! Current machine state.
    State curr_state_ { 0 };

    //! Next machine state. Set to the required state if necessary.
    State next_state_ { 0 };

    //! Time spend in the previous machine state.
    system::Time prev_state_dur_ { 0 };

    //! Time spend in the current machine state.
    system::Time curr_state_dur_ { 0 };

    // Number of time a block was saved to the storage.
    uint64_t write_count_ { 0 };

    system::Time start_ts_ { 0 };
    system::Time saved_ts_ { 0 };
};

} // namespace control
} // namespace ocs
