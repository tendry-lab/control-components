/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "driver/i2c_master.h"
#include "driver/i2c_types.h"

#include "ocs_core/log.h"
#include "ocs_io/i2c/target_esp32/master_bus.h"
#include "ocs_io/i2c/target_esp32/master_transceiver.h"

namespace ocs {
namespace io {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_bus";

} // namespace

MasterBus::MasterBus(i2c_master_bus_handle_t handle)
    : handle_(handle) {
}

IBus::ITransceiverPtr
MasterBus::add(AddressLength len, Address addr, TransferSpeed speed) {
    i2c_device_config_t config;
    memset(&config, 0, sizeof(config));

#if SOC_I2C_SUPPORT_10BIT_ADDR
    if (len == AddressLength::Bit_10) {
        config.dev_addr_length = I2C_ADDR_BIT_LEN_10;
    } else {
        config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    }
#else  // !SOC_I2C_SUPPORT_10BIT_ADDR
    configASSERT(len == AddressLength::Bit_7);
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
#endif // SOC_I2C_SUPPORT_10BIT_ADDR

    config.device_address = addr;

    if (speed == TransferSpeed::Fast) {
        config.scl_speed_hz = 400000;
    } else {
        config.scl_speed_hz = 100000;
    }

    i2c_master_dev_handle_t device = nullptr;

    auto err = i2c_master_bus_add_device(handle_, &config, &device);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "i2c_master_bus_add_device() failed: addr=0x%" PRIx16 " err=%s",
                 addr, esp_err_to_name(err));

        return nullptr;
    }

    auto device_ptr = MasterTransceiver::make_device_shared(device);
    configASSERT(device_ptr);

    return IBus::ITransceiverPtr(new (std::nothrow)
                                     MasterTransceiver(handle_, device_ptr, addr));
}

} // namespace i2c
} // namespace io
} // namespace ocs
