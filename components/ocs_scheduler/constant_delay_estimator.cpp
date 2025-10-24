/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/constant_delay_estimator.h"

namespace ocs {
namespace scheduler {

ConstantDelayEstimator::ConstantDelayEstimator(TickType_t delay)
    : delay_(delay) {
}

void ConstantDelayEstimator::begin() {
}

TickType_t ConstantDelayEstimator::estimate() {
    return delay_;
}

} // namespace scheduler
} // namespace ocs
