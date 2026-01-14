/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace test {

class TestGpio : public io::gpio::IGpio, private core::NonCopyable<> {
public:
    TestGpio(status::StatusCode flip_code,
             status::StatusCode turn_on_code,
             status::StatusCode turn_off_code);

    status::StatusCode get_level(io::gpio::Level& level) override;
    status::StatusCode flip() override;
    status::StatusCode turn_on() override;
    status::StatusCode turn_off() override;
    status::StatusCode set_direction(io::gpio::Direction direction) override;

    size_t flip_call_count { 0 };
    size_t turn_on_call_count { 0 };
    size_t turn_off_call_count { 0 };
    io::gpio::Direction direction { io::gpio::Direction::Output };

private:
    io::gpio::Level level_ { 0 };

    status::StatusCode flip_code_ { status::StatusCode::OK };
    status::StatusCode turn_on_code_ { status::StatusCode::OK };
    status::StatusCode turn_off_code_ { status::StatusCode::OK };
};

} // namespace test
} // namespace ocs
