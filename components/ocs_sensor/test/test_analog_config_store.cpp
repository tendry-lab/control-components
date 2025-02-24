/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_sensor/analog_config_store.h"
#include "ocs_test/memory_storage.h"

namespace ocs {
namespace sensor {

TEST_CASE("Analog config store: get", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage storage;

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    AnalogConfigStore store;
    TEST_ASSERT_NULL(store.get(id));

    store.add(config);

    const auto recv_config = store.get(id);
    TEST_ASSERT_NOT_NULL(recv_config);
    TEST_ASSERT_EQUAL_STRING(config.get_id(), recv_config->get_id());
}

TEST_CASE("Analog config store: get all", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id1 = "test_1";
    const char* id2 = "test_2";

    test::MemoryStorage storage;

    AnalogConfig config1(storage, def_min, def_max, id1);
    AnalogConfig config2(storage, def_min, def_max, id2);

    AnalogConfigStore store;

    store.add(config1);
    store.add(config2);
    TEST_ASSERT_NOT_NULL(store.get(id1));
    TEST_ASSERT_NOT_NULL(store.get(id2));

    const auto configs = store.get_all();
    TEST_ASSERT_EQUAL(2, configs.size());

    TEST_ASSERT_EQUAL_STRING(config1.get_id(), configs[0]->get_id());
    TEST_ASSERT_EQUAL_STRING(config2.get_id(), configs[1]->get_id());
}

} // namespace sensor
} // namespace ocs
