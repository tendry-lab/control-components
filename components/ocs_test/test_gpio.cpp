/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/test_gpio.h"

namespace ocs {
namespace test {

TestGpio::TestGpio(status::StatusCode flip_code,
                   status::StatusCode turn_on_code,
                   status::StatusCode turn_off_code)
    : flip_code_(flip_code)
    , turn_on_code_(turn_on_code)
    , turn_off_code_(turn_off_code) {
}

status::StatusCode TestGpio::get_level(io::gpio::Level& level) {
    level = level_;

    return status::StatusCode::OK;
}

status::StatusCode TestGpio::flip() {
    level_ = !level_;

    ++flip_call_count;

    return flip_code_;
}

status::StatusCode TestGpio::turn_on() {
    ++turn_on_call_count;

    level_ = 1;

    return turn_on_code_;
}

status::StatusCode TestGpio::turn_off() {
    ++turn_off_call_count;

    level_ = 0;

    return turn_off_code_;
}

status::StatusCode TestGpio::set_direction(io::gpio::Direction direction) {
    this->direction = direction;
    return status::StatusCode::OK;
}

} // namespace test
} // namespace ocs
