/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace system {

//! Microseconds.
using Time = int64_t;

//! Common durations.
//!
//! To count the number of units in a Duration, divide:
//!
//! const auto second = Duration::second;
//! print(second / Duration::millisecond)) // prints 1000
//!
//! To convert an integer number of units to a Duration, multiply:
//!
//! const unsigned seconds = 10;
//! print(seconds * Duration::second) // prints 10 * 1000 * 1000
struct Duration {
    //! One microsecond represented in microseconds.
    static constexpr Time microsecond = 1;

    //! One millisecond represented in microseconds.
    static constexpr Time millisecond = 1000 * microsecond;

    //! One second represented in microseconds.
    static constexpr Time second = 1000 * millisecond;

    //! One minute represented in microseconds.
    static constexpr Time minute = 60 * second;

    //! One hour represented in microseconds.
    static constexpr Time hour = 60 * minute;
};

} // namespace system
} // namespace ocs
