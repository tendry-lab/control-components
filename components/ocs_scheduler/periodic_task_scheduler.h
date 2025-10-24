/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/rate_limiter.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace scheduler {

class PeriodicTaskScheduler : public ITaskScheduler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to measure the time taken for a single round of execution.
    //!  - @p estimator to estimate the required delay after each round of execution.
    //!  - @p id to distinguish one scheduler from another.
    //!  - @p max_count - maximum number of tasks the scheduler can handle.
    PeriodicTaskScheduler(system::IClock& clock,
                          IDelayEstimator& estimator,
                          const char* id,
                          unsigned max_count);

    //! Return the maximum configured number of tasks a scheduler can handle.
    unsigned max_count() const override;

    //! Add task to be executed periodically once per interval.
    //!
    //! @params
    //!  - @p task - task to be executed periodically.
    //!  - @p id - unique task identifier.
    //!  - @p interval - task running frequency, milliseconds resolution.
    //!
    //! @notes
    //! It's possible for the task's frequency to drift over time. There are several
    //! reasons for this:
    //!  - This scheduler can't be considered as a precise scheduler, since all tasks
    //!    are running on the same FreeRTOS task. This task can be preempted by other
    //!    tasks with higher priority.
    //!  - If there are too many tasks added to the same scheduler, it is possible that
    //!    the total time required to run all these tasks will be greater then the
    //!    minimum periodic interval.
    status::StatusCode add(ITask& task, const char* id, system::Time interval) override;

    //! Remove task by @p id.
    status::StatusCode remove(const char* id) override;

    //! Start tasks scheduling.
    status::StatusCode start() override;

    //! Stop tasks scheduling.
    status::StatusCode stop() override;

    //! Run all periodic tasks.
    status::StatusCode run() override;

private:
    class Node : public ITask, private core::NonCopyable<> {
    public:
        Node(system::IClock& clock, ITask& task, const char* id, system::Time interval);

        status::StatusCode run() override;

        const char* id() const;
        bool alive() const;

        void set_alive(bool alive);

    private:
        const std::string id_;

        ITask& task_;

        core::RateLimiter limiter_;
        bool alive_ { false };
    };

    using NodePtr = std::shared_ptr<Node>;
    using NodeList = std::vector<NodePtr>;

    static NodePtr node_exist_(const NodeList& nodes, const char* id);
    static void node_remove_(NodeList& nodes, const char* id);

    void run_();

    const unsigned max_count_ { 0 };
    const std::string log_tag_;

    system::Time total_ts_min_ { INT64_MAX };
    system::Time total_ts_max_ { INT64_MIN };

    system::IClock& clock_;
    IDelayEstimator& estimator_;

    NodeList nodes_to_add_;
    NodeList nodes_to_remove_;
    NodeList nodes_all_;
};

} // namespace scheduler
} // namespace ocs
