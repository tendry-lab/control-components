/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_storage/storage_builder.h"
#include "ocs_storage/target_esp32/flash_initializer.h"

namespace ocs {
namespace storage {

TEST_CASE("Storage building validation", "[ocs_storage], [storage_builder]") {
    FlashInitializer initializer;

    StorageBuilder builder;

    const char* key = "foo";

    TEST_ASSERT_NOT_NULL(builder.make(key));
    TEST_ASSERT_NULL(builder.make(key));
}

} // namespace storage
} // namespace ocs
