/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/target_esp32/ap_network.h"
#include "ocs_net/target_esp32/default_mdns_server.h"
#include "ocs_storage/target_esp32/flash_initializer.h"

namespace ocs {
namespace net {

namespace {

struct TestStorage : public storage::IStorage {
    status::StatusCode probe(const char* key, size_t& size) override {
        return status::StatusCode::Error;
    }

    status::StatusCode read(const char* key, void* value, size_t size) override {
        return status::StatusCode::Error;
    }

    status::StatusCode write(const char* key, const void* value, size_t size) override {
        return status::StatusCode::Error;
    }

    status::StatusCode erase(const char* key) override {
        return status::StatusCode::Error;
    }

    status::StatusCode erase_all() override {
        return status::StatusCode::Error;
    }
};

} // namespace

TEST_CASE("Default mDNS server: start/stop", "[ocs_net], [default_mdns_server]") {
    storage::FlashInitializer flash_initializer;
    FanoutNetworkHandler handler;

    TestStorage storage;
    system::DeviceInfo device_info("test-firmware", "0.0.0", "Test Firmware", "product");
    ApNetworkConfig config(storage, device_info);

    ApNetwork network(handler, config);

    DefaultMdnsServer mdns_server("host");

    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_server.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, mdns_server.stop());
}

} // namespace net
} // namespace ocs
