/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_system/iarena.h"
#include "ocs_system/itimer.h"
#include "ocs_system/platform_builder.h"

namespace ocs {
namespace scheduler {

class AsyncTaskScheduler : public ITaskScheduler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p platform_builder to build high-resolution timers.
    //!  - @p estimator to estimate the required delay after each round of execution.
    //!  - @p id to distinguish one scheduler from another.
    AsyncTaskScheduler(system::IArena& arena,
                       system::PlatformBuilder& platform_builder,
                       IDelayEstimator& estimator,
                       const char* id);

    //! Maximum number of tasks to which a scheduler can deliver asynchronous events.
    size_t max_count() const override;

    //! Add task to be executed once per interval.
    //!
    //! @params
    //!  - @p task to which asynchronous events should be scheduled.
    //!  - @p id to distinguish one task from another.
    //!  - @p interval - task running frequency, microseconds resolution.
    //!
    //! @notes
    //!  This scheduler can be considered as a precise scheduler. Once per interval
    //!  high-resolution timer schedules the task execution, which is handled during the
    //!  run() call.
    status::StatusCode add(ITask& task, const char* id, system::Time interval) override;

    //! Remove task by @p id.
    status::StatusCode remove(const char* id) override;

    //! Start tasks scheduling.
    status::StatusCode start() override;

    //! Stop tasks scheduling.
    status::StatusCode stop() override;

    //! Wait for the asynchronous tasks.
    status::StatusCode run() override;

    //! Return an event group to schedule external asynchronous events.
    //!
    //! @remarks
    //!  Use only for the events allocated by the attach() API.
    EventGroupHandle_t get_event_group();

    //! Attach an externally clocked task.
    //!
    //! @params
    //!  - @p event to dispatch asynchronous events to the @p task.
    //!  - @p task - to be called when an asynchronous event happens.
    //!  - @p id - unique task identifier, to distinguish one task from another.
    //!
    //! @remarks
    //!  The add() API uses an external high-resolution clock to periodically trigger
    //!  an asynchronous event, while the attach() API is used when an asynchronous
    //!  event is triggered by an external source.
    status::StatusCode attach(EventBits_t& event, ITask& task, const char* id);

private:
    class Node : public ITask, private core::NonCopyable<> {
    public:
        enum class ClockType : uint8_t {
            //! Node based on the high-resolution timer.
            Internal,

            //! Externally clocked node.
            External,
        };

        Node(ITask& task, EventBits_t event, const char* id, ClockType clock_type);

        Node(system::IArena& arena,
             system::PlatformBuilder& platform_builder,
             ITask& task,
             EventGroupHandle_t event_group,
             EventBits_t event,
             system::Time interval,
             const char* id);

        status::StatusCode run() override;

        const char* id() const;
        EventBits_t event() const;
        ClockType get_clock_type() const;

        status::StatusCode start();
        status::StatusCode stop();

    private:
        const std::string id_;
        const EventBits_t event_ { 0 };
        const ClockType clock_type_ { ClockType::Internal };

        ITask& task_;

        system::UniquePtr<ITask> async_task_;
        system::PlatformBuilder::ITimerPtr timer_;
    };

    status::StatusCode allocate_event_(EventBits_t& event, const char* id);

    void run_(EventBits_t bits);

    const std::string log_tag_;

    system::IArena& arena_;
    system::PlatformBuilder& platform_builder_;
    IDelayEstimator& estimator_;

    using NodePtr = std::shared_ptr<Node>;
    std::vector<NodePtr> nodes_;

    core::StaticEventGroup event_group_;
    EventBits_t bits_all_ { 0 };
};

} // namespace scheduler
} // namespace ocs
