/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_core/log.h"
#include "ocs_io/gpio/default_gpio.h"
#include "ocs_io/i2c/target_esp32/master_store.h"
#include "ocs_sensor/sht4x/sensor.h"
#include "ocs_status/code_to_str.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_storage/target_esp32/flash_initializer.h"
#include "ocs_system/time.h"

using namespace ocs;

namespace {

struct VerificationConfig {
    //! How long to wait for the I2C operation to complete.
    system::Time i2c_wait_timeout { system::Duration::second * 5 };

    //! Sensor I2C address.
    io::i2c::Address i2c_addr { 0x44 };

    //! I2C transfer speed.
    io::i2c::TransferSpeed i2c_transfer_speed { io::i2c::TransferSpeed::Fast };

    //! How long to wait before receiving the operation result from the I2C device.
    TickType_t i2c_delay_interval { pdMS_TO_TICKS(10) };

    //! How long to wait after each sensor reading.
    TickType_t read_wait_interval { pdMS_TO_TICKS(1000) };

    //! Number of times to read data from the sensor.
    size_t total_attempts { 15 };
};

const char* log_tag = "sht4x_verifier";

void configure_power_gpio(io::gpio::Gpio gpio) {
    gpio_config_t config;
    memset(&config, 0, sizeof(config));

    config.intr_type = GPIO_INTR_DISABLE;
    config.mode = GPIO_MODE_OUTPUT;

    config.pin_bit_mask = algo::BitOps::mask(gpio);

    config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    config.pull_up_en = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&config));
}

void enable_power(io::gpio::Gpio gpio) {
    io::gpio::DefaultGpio power_gpio("power", gpio);

    configASSERT(power_gpio.turn_on() == status::StatusCode::OK);

    static constexpr TickType_t stabilization_interval = pdMS_TO_TICKS(200);

    ocs_logi(log_tag, "power stabilization: wait for %lu(ms)",
             pdTICKS_TO_MS(stabilization_interval));

    vTaskDelay(stabilization_interval);

    ocs_logi(log_tag, "power stabilization: completed");
}

void disable_power(io::gpio::Gpio gpio) {
    io::gpio::DefaultGpio power_gpio("power", gpio);

    configASSERT(power_gpio.turn_off() == status::StatusCode::OK);
}

void perform_verification(const VerificationConfig& verification_config) {
    std::unique_ptr<io::i2c::IStore> store(new (
        std::nothrow) io::i2c::MasterStore(io::i2c::MasterStore::Params {
        .sda =
            static_cast<gpio_num_t>(CONFIG_OCS_TOOL_SHT4x_VERIFIER_I2C_MASTER_SDA_GPIO),
        .scl =
            static_cast<gpio_num_t>(CONFIG_OCS_TOOL_SHT4x_VERIFIER_I2C_MASTER_SCL_GPIO),
    }));
    configASSERT(store);

    io::i2c::IStore::ITransceiverPtr bus_transceiver = store->add(
        "bus", io::i2c::AddressLength::Bit_7, io::i2c::IStore::bus_fanout_address,
        verification_config.i2c_transfer_speed);
    configASSERT(bus_transceiver);

    auto code = bus_transceiver->send(&io::i2c::IStore::bus_reset_command,
                                      sizeof(io::i2c::IStore::bus_reset_command),
                                      verification_config.i2c_wait_timeout);
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to reset i2c bus: %s", status::code_to_str(code));
    }

    io::i2c::IStore::ITransceiverPtr sensor_transceiver =
        store->add("sht4x", io::i2c::AddressLength::Bit_7, verification_config.i2c_addr,
                   verification_config.i2c_transfer_speed);
    configASSERT(sensor_transceiver);

    storage::FlashInitializer flash_initializer;

    std::unique_ptr<storage::StorageBuilder> storage_builder(
        new (std::nothrow) storage::StorageBuilder());
    configASSERT(storage_builder);

    auto storage = storage_builder->make("sensor_sht4x");

    std::unique_ptr<sensor::sht4x::Sensor> sensor(
        new (std::nothrow) sensor::sht4x::Sensor(
            *sensor_transceiver, *storage, "sht4x",
            sensor::sht4x::Sensor::Params {
                .i2c_delay_interval = verification_config.i2c_delay_interval,
                .i2c_wait_timeout = verification_config.i2c_wait_timeout,
                .measure_command = sensor::sht4x::Sensor::Command::MeasureHighPrecision,
            }));
    configASSERT(sensor);

    code = sensor->reset();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to reset sensor: %s", status::code_to_str(code));
    }

    size_t failed_attempts = 0;

    for (size_t n = 0; n < verification_config.total_attempts; ++n) {
        const auto code = sensor->run();
        if (code != status::StatusCode::OK) {
            ++failed_attempts;
            continue;
        }

        const auto data = sensor->get_data();

        ocs_logi(log_tag, "temperature=%.2f humidity=%.2f", data.temperature,
                 data.humidity);

        vTaskDelay(verification_config.read_wait_interval);
    }

    ocs_logi(log_tag, "verification finished: total=%u failed=%u",
             verification_config.total_attempts, failed_attempts);
}

} // namespace

extern "C" void app_main(void) {
    const io::gpio::Gpio power_gpio =
        static_cast<io::gpio::Gpio>(CONFIG_OCS_TOOL_SHT4x_VERIFIER_POWER_GPIO);
    if (power_gpio >= 0) {
        configure_power_gpio(power_gpio);
        enable_power(power_gpio);
    }

    VerificationConfig verification_config;
    perform_verification(verification_config);

    if (power_gpio >= 0) {
        disable_power(power_gpio);
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
