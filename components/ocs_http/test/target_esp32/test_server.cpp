/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>
#include <string>

#include "unity.h"

#include "ocs_algo/response_ops.h"
#include "ocs_http/router.h"
#include "ocs_http/target_esp32/client_reader.h"
#include "ocs_http/target_esp32/server.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_net/ip_addr_to_str.h"
#include "ocs_net/target_esp32/sta_network.h"
#include "ocs_storage/target_esp32/flash_initializer.h"
#include "ocs_test/memory_storage.h"

#ifdef CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED
#include "ocs_core/istream_reader.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_http/chunk_stream_writer.h"
#endif // CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED

namespace ocs {
namespace http {

TEST_CASE("Stop HTTP server: no start", "[ocs_http], [server]") {
    Router router;
    Server server(router, Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: WiFi not started", "[ocs_http], [server]") {
    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    test::MemoryStorage storage;

    { // Configure the network first.
        net::StaNetworkConfig config(storage);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, config.configure(1, "foo", "12345678"));
    }

    net::StaNetworkConfig config(storage);
    net::StaNetwork network(handler, config);

    Router router;
    Server server(router, Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
}

TEST_CASE("Start HTTP server: WiFi invalid credentials", "[ocs_http], [server]") {
    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    test::MemoryStorage storage;

    { // Configure the network first.
        net::StaNetworkConfig config(storage);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, config.configure(1, "foo", "12345678"));
    }

    net::StaNetworkConfig config(storage);
    net::StaNetwork network(handler, config);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::Error, network.wait());

    Router router;
    Server server(router, Server::Params {});
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}

#ifdef CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED
TEST_CASE("Start HTTP server: WiFi valid credentials", "[ocs_http], [server]") {
    const unsigned server_port = 80;

    Router router;
    Server server(router,
                  Server::Params {
                      .server_port = server_port,
                  });

    const char* path = "/foo";
    const char* want_response = "hello world";

    class TestHandler : public IHandler {
    public:
        explicit TestHandler(const char* response)
            : response_(response) {
        }

        status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) {
            return algo::ResponseOps::write_text(w, response_.c_str());
        }

    private:
        std::string response_;
    } http_handler(want_response);

    router.add(http::IRouter::Method::Get, path, http_handler);

    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;
    test::MemoryStorage storage;

    { // Configure the network first.
        net::StaNetworkConfig config(storage);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          config.configure(1, CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID,
                                           CONFIG_OCS_TEST_UNIT_WIFI_STA_PASSWORD));
    }

    net::StaNetworkConfig config(storage);
    net::StaNetwork network(handler, config);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());

    const auto info = network.get_info();

    net::ip_addr_to_str ip_addr_str(info.ip_addr);

    ClientReader reader(ClientReader::Params {
        .host = ip_addr_str.c_str(),
        .path = path,
    });
    TEST_ASSERT_EQUAL(ESP_OK, esp_http_client_perform(reader.client()));
    TEST_ASSERT_EQUAL(200, esp_http_client_get_status_code(reader.client()));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, reader.wait());

    char got_response[strlen(want_response) + 1];
    memset(got_response, 0, sizeof(got_response));
    TEST_ASSERT_EQUAL(strlen(want_response),
                      reader.read(got_response, strlen(want_response)));

    TEST_ASSERT_EQUAL_STRING(want_response, got_response);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}

TEST_CASE("Start HTTP server: chunked response", "[ocs_http], [server]") {
    struct TestStreamReader : public core::IStreamReader {
        TestStreamReader(const char* response, unsigned chunk_size)
            : response_(response)
            , chunk_size_(chunk_size) {
        }

        status::StatusCode begin() override {
            return status::StatusCode::OK;
        }

        status::StatusCode end() override {
            return status::StatusCode::OK;
        }

        status::StatusCode cancel() override {
            return status::StatusCode::OK;
        }

        status::StatusCode read(void* data, unsigned& size) override {
            if (pos_ == response_.size()) {
                return status::StatusCode::NoData;
            }

            const auto remain = response_.size() - pos_;
            size = std::min(remain, chunk_size_);

            memcpy(data, response_.data() + pos_, size);
            pos_ += size;

            return status::StatusCode::OK;
        }

    private:
        const std::string response_;
        const unsigned chunk_size_ { 0 };

        unsigned pos_ { 0 };
    };

    const unsigned server_port = 80;

    Router router;
    Server server(router,
                  Server::Params {
                      .server_port = server_port,
                  });

    const char* path = "/foo";
    const char* want_response = "hello world";

    class TestHandler : public IHandler {
    public:
        explicit TestHandler(const char* response)
            : response_(response) {
        }

        status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) {
            const auto code = w.get_header().set("Transfer-Encoding", "chunked");
            if (code != status::StatusCode::OK) {
                return code;
            }

            core::StreamTransceiver::Buffer buffer;
            buffer.resize(response_.size());

            ChunkStreamWriter writer(w);
            TestStreamReader reader(response_.c_str(), 1);

            core::StreamTransceiver transceiver(reader, writer, buffer);
            return transceiver.transceive();
        }

    private:
        std::string response_;
    } http_handler(want_response);

    router.add(http::IRouter::Method::Get, path, http_handler);

    storage::FlashInitializer flash_initializer;
    net::FanoutNetworkHandler handler;

    test::MemoryStorage storage;

    { // Configure the network first.
        net::StaNetworkConfig config(storage);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          config.configure(1, CONFIG_OCS_TEST_UNIT_WIFI_STA_SSID,
                                           CONFIG_OCS_TEST_UNIT_WIFI_STA_PASSWORD));
    }

    net::StaNetworkConfig config(storage);
    net::StaNetwork network(handler, config);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.start());

    const auto info = network.get_info();

    net::ip_addr_to_str ip_addr_str(info.ip_addr);

    ClientReader reader(ClientReader::Params {
        .host = ip_addr_str.c_str(),
        .path = path,
    });
    TEST_ASSERT_EQUAL(ESP_OK, esp_http_client_perform(reader.client()));
    TEST_ASSERT_EQUAL(200, esp_http_client_get_status_code(reader.client()));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, reader.wait());

    char got_response[strlen(want_response) + 1];
    memset(got_response, 0, sizeof(got_response));
    TEST_ASSERT_EQUAL(strlen(want_response),
                      reader.read(got_response, strlen(want_response)));

    TEST_ASSERT_EQUAL_STRING(want_response, got_response);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, server.stop());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, network.stop());
}
#endif // CONFIG_OCS_TEST_UNIT_WIFI_STA_ENABLED

} // namespace http
} // namespace ocs
