/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/idelay_estimator.h"

namespace ocs {
namespace scheduler {

class ConstantDelayEstimator : public IDelayEstimator, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit ConstantDelayEstimator(TickType_t delay);

    //! Begin delay estimation.
    void begin() override;

    //! Return the configured delay.
    TickType_t estimate() override;

private:
    const TickType_t delay_ { pdMS_TO_TICKS(0) };
};

} // namespace scheduler
} // namespace ocs
