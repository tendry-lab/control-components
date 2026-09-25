/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/ivalue_monitor.h"
#include "ocs_core/noncopyable.h"
#include "ocs_status/code.h"

namespace ocs {
namespace core {

//! Value monitor that delegates the update to the provided function.
template <typename T>
class FuncValueMonitor : public IValueMonitor<T>, private NonCopyable<> {
public:
    //! Function to update the underlying monitor.
    using Func = std::function<status::StatusCode()>;

    //! Initialize.
    //!
    //! @params
    //!  - @p monitor to get the latest sampled value.
    //!  - @p func to update the @p monitor, e.g. asynchronously on a specific task.
    FuncValueMonitor(IValueMonitor<T>& monitor, Func func)
        : monitor_(monitor)
        , func_(func) {
    }

    //! Get latest sampled value from the underlying monitor.
    T get() const override {
        return monitor_.get();
    }

    //! Call the function to update the underlying monitor.
    status::StatusCode update() override {
        return func_();
    }

private:
    IValueMonitor<T>& monitor_;
    Func func_;
};

} // namespace core
} // namespace ocs
