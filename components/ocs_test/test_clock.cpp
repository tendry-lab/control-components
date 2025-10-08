/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_test/test_clock.h"

namespace ocs {
namespace test {

system::Time TestClock::now() {
    return value;
}

} // namespace test
} // namespace ocs
