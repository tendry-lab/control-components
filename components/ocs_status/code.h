/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace status {

//! Status code.
enum class StatusCode : uint8_t {
    //! Status indicating a success of an operation.
    OK,

    //! Status indicating a failure of an operation.
    Error,

    //! There is no enough data to perform an operation.
    NoData,

    //! There is no enough memory to perform an operation.
    NoMem,

    //! A context switch should be requested before the interrupt is exited.
    IsrYield,

    //! An operation can't be performed due to invalid arguments.
    InvalidArg,

    //! An operation can't be performed due to invalid state.
    InvalidState,

    //! An operation was not performed within the timeout.
    Timeout,

    //! An operation hasn't changed the resource.
    NotModified,

    //! An operation isn't supported.
    NotSupported,

    //! Invalid status code.
    Last,
};

} // namespace status
} // namespace ocs
