/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/sta_network.h"
#include "ocs_storage/flash_initializer.h"
#include "ocs_test/memory_storage.h"

namespace ocs {
namespace net {

#ifdef CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED
TEST_CASE("WiFi STA: connect to AP: invalid credentials",
          "[ocs_net], [sta_network| update]") {
    { // Invalid SSID
        storage::FlashInitializer flash_initializer;
        FanoutNetworkHandler handler;
        test::MemoryStorage storage;

        { // Configure the network first.
            StaNetworkConfig config(storage);
            TEST_ASSERT_EQUAL(
                status::StatusCode::OK,
                config.configure(1, "foo", CONFIG_OCS_TEST_UNIT_WIFI_STA_PASSWORD));
        }

        StaNetworkConfig config(storage);
        StaNetwork network(handler, config);

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
    { // Invalid password
        storage::FlashInitializer flash_initializer;
        FanoutNetworkHandler handler;
        test::MemoryStorage storage;

        { // Configure the network first.
            StaNetworkConfig config(storage);
            TEST_ASSERT_EQUAL(
                status::StatusCode::OK,
                config.configure(1, CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID, "12345678"));
        }

        StaNetworkConfig config(storage);
        StaNetwork network(handler, config);

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
    { // Invalid SSID and password
        storage::FlashInitializer flash_initializer;
        FanoutNetworkHandler handler;
        test::MemoryStorage storage;

        { // Configure the network first.
            StaNetworkConfig config(storage);
            TEST_ASSERT_EQUAL(status::StatusCode::OK,
                              config.configure(1, "foo", "12345678"));
        }

        StaNetworkConfig config(storage);
        StaNetwork network(handler, config);

        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
        TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
    }
}

TEST_CASE("WiFi STA: connect to AP: valid credentials",
          "[ocs_net], [sta_network| update]") {
    storage::FlashInitializer flash_initializer;
    FanoutNetworkHandler handler;
    test::MemoryStorage storage;

    { // Configure the network first.
        StaNetworkConfig config(storage);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          config.configure(3, CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID,
                                           CONFIG_OCS_TEST_UNIT_WIFI_STA_PASSWORD));
    }

    StaNetworkConfig config(storage);
    StaNetwork network(handler, config);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.wait());

    wifi_ap_record_t record;
    memset(&record, 0, sizeof(record));
    TEST_ASSERT_EQUAL(ESP_OK, esp_wifi_sta_get_ap_info(&record));
    TEST_ASSERT_EQUAL_STRING(CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID, record.ssid);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}
#endif // CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED

} // namespace net
} // namespace ocs
