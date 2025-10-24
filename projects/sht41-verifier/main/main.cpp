/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/i2c/target_esp32/master_store_pipeline.h"
#include "ocs_sensor/sht41/sensor.h"
#include "ocs_storage/storage_builder.h"

using namespace ocs;

namespace {

const char* log_tag = "sht41_verifier";

} // namespace

extern "C" void app_main(void) {
    std::unique_ptr<io::i2c::MasterStorePipeline> store_pipeline(
        new (std::nothrow) io::i2c::MasterStorePipeline(io::i2c::IStore::Params {
            .sda = static_cast<io::gpio::Gpio>(
                CONFIG_OCS_TOOL_SHT41_VERIFIER_I2C_MASTER_SDA_GPIO),
            .scl = static_cast<io::gpio::Gpio>(
                CONFIG_OCS_TOOL_SHT41_VERIFIER_I2C_MASTER_SCL_GPIO),
        }));
    configASSERT(store_pipeline);

    io::i2c::IStore::ITransceiverPtr transceiver =
        store_pipeline->get_store().add("sht41", io::i2c::IStore::AddressLength::Bit_7,
                                        0x44, io::i2c::IStore::TransferSpeed::Fast);
    configASSERT(transceiver);

    std::unique_ptr<storage::StorageBuilder> storage_builder(
        new (std::nothrow) storage::StorageBuilder());
    configASSERT(storage_builder);

    auto storage = storage_builder->make("sensor_sht41");

    std::unique_ptr<sensor::sht41::Sensor> sensor(
        new (std::nothrow) sensor::sht41::Sensor(
            *transceiver, *storage,
            sensor::sht41::Sensor::Params {
                .send_wait_interval =
                    pdMS_TO_TICKS(CONFIG_OCS_TOOLS_SHT41_VERIFIER_I2C_SEND_WAIT_INTERVAL),
                .bus_wait_interval = system::Duration::second * 5,
                .measure_command = sensor::sht41::Sensor::Command::MeasureHighPrecision,
            }));
    configASSERT(sensor);

    const unsigned total_attempts = CONFIG_OCS_TOOLS_SHT41_VERIFIER_TOTAL_ATTEMPTS;
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

        vTaskDelay(pdMS_TO_TICKS(1000 * CONFIG_OCS_TOOLS_SHT41_VERIFIER_DELAY_INTERVAL));
    }

    ocs_logi(log_tag, "verification finished: total=%u failed=%u", total_attempts,
             failed_attempts);

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
