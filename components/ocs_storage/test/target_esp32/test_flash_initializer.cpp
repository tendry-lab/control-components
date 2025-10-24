/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_storage/target_esp32/flash_initializer.h"

namespace ocs {
namespace storage {

TEST_CASE("Flash initializer", "[ocs_storage], [flash_initializer]") {
    FlashInitializer initializer;
}

} // namespace storage
} // namespace ocs
