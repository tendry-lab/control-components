/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/i2c/target_esp32/master_store.h"
#include "ocs_sensor/sht4x/sensor.h"
#include "ocs_status/code_to_str.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/time.h"

using namespace ocs;

namespace {

const char* log_tag = "sht4x_verifier";

} // namespace

extern "C" void app_main(void) {
    const system::Time bus_wait_interval = system::Duration::second * 5;

    std::unique_ptr<io::i2c::IStore> store(
        new (std::nothrow) io::i2c::MasterStore(io::i2c::IStore::Params {
            .sda = static_cast<io::gpio::Gpio>(
                CONFIG_OCS_TOOL_SHT4x_VERIFIER_I2C_MASTER_SDA_GPIO),
            .scl = static_cast<io::gpio::Gpio>(
                CONFIG_OCS_TOOL_SHT4x_VERIFIER_I2C_MASTER_SCL_GPIO),
        }));
    configASSERT(store);

    io::i2c::IStore::ITransceiverPtr bus_transceiver = store->add(
        "bus", io::i2c::IStore::AddressLength::Bit_7, io::i2c::IStore::bus_fanout_address,
        io::i2c::IStore::TransferSpeed::Fast);
    configASSERT(bus_transceiver);

    auto code = bus_transceiver->send(&io::i2c::IStore::bus_reset_command,
                                      sizeof(io::i2c::IStore::bus_reset_command),
                                      bus_wait_interval);
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to reset i2c bus: %s", status::code_to_str(code));
    }

    io::i2c::IStore::ITransceiverPtr sensor_transceiver =
        store->add("sht4x", io::i2c::IStore::AddressLength::Bit_7, 0x44,
                   io::i2c::IStore::TransferSpeed::Fast);
    configASSERT(sensor_transceiver);

    std::unique_ptr<storage::StorageBuilder> storage_builder(
        new (std::nothrow) storage::StorageBuilder());
    configASSERT(storage_builder);

    auto storage = storage_builder->make("sensor_sht4x");

    std::unique_ptr<sensor::sht4x::Sensor> sensor(
        new (std::nothrow) sensor::sht4x::Sensor(
            *sensor_transceiver, *storage, "sht4x",
            sensor::sht4x::Sensor::Params {
                .send_wait_interval =
                    pdMS_TO_TICKS(CONFIG_OCS_TOOLS_SHT4x_VERIFIER_I2C_SEND_WAIT_INTERVAL),
                .bus_wait_interval = system::Duration::second * 5,
                .measure_command = sensor::sht4x::Sensor::Command::MeasureHighPrecision,
            }));
    configASSERT(sensor);

    code = sensor->reset();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to reset sensor: %s", status::code_to_str(code));
    }

    const unsigned total_attempts = CONFIG_OCS_TOOLS_SHT4x_VERIFIER_TOTAL_ATTEMPTS;
    unsigned failed_attempts = 0;

    for (unsigned n = 0; n < total_attempts; ++n) {
        const auto code = sensor->run();
        if (code != status::StatusCode::OK) {
            ++failed_attempts;
            continue;
        }

        const auto data = sensor->get_data();

        ocs_logi(log_tag, "temperature=%.2f humidity=%.2f", data.temperature,
                 data.humidity);

        vTaskDelay(pdMS_TO_TICKS(1000 * CONFIG_OCS_TOOLS_SHT4x_VERIFIER_DELAY_INTERVAL));
    }

    ocs_logi(log_tag, "verification finished: total=%u failed=%u", total_attempts,
             failed_attempts);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
