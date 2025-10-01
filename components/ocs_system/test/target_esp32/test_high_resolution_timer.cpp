/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_system/target_esp32/high_resolution_timer.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace system {

TEST_CASE("High resolution timer: handle events in periodic task",
          "[ocs_system], [high_resolution_timer]") {
    test::TestTask task(status::StatusCode::OK);
    HighResolutionTimer timer(task, "test", core::Duration::millisecond * 100);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, timer.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task.wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, timer.stop());
}

} // namespace system
} // namespace ocs
