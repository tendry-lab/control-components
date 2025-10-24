/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

namespace ocs {
namespace scheduler {

//! Estimate required delay of FreeRTOS task.
class IDelayEstimator {
public:
    //! Destroy.
    virtual ~IDelayEstimator() = default;

    //! Begin delay estimation.
    virtual void begin() = 0;

    //! Estimate required delay.
    virtual TickType_t estimate() = 0;
};

} // namespace scheduler
} // namespace ocs
