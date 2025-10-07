/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_control/system_fsm.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace control {

namespace {

struct TestRebooter : public system::IRebooter, private core::NonCopyable<> {
    unsigned call_count { 0 };

    void reboot() override {
        ++call_count;
    }
};

} // namespace

TEST_CASE("System FSM: button is released within interval",
          "[system_fsm], [ocs_control]") {
    TestRebooter rebooter;

    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    SystemFsm fsm(rebooter, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(1, rebooter.call_count);
}

TEST_CASE("System FSM: button isn't released within interval",
          "[system_fsm], [ocs_control]") {
    TestRebooter rebooter;

    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval * 2;

    SystemFsm fsm(rebooter, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, rebooter.call_count);
}

} // namespace control
} // namespace ocs
