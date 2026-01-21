/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_io/i2c/target_esp32/master_transceiver.h"
#include "ocs_core/log.h"

namespace ocs {
namespace io {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_transceiver";

} // namespace

MasterTransceiver::DevicePtr
MasterTransceiver::make_device_shared(i2c_master_dev_handle_t device) {
    return MasterTransceiver::DevicePtr(device, [](i2c_master_dev_handle_t device) {
        if (device) {
            const auto err = i2c_master_bus_rm_device(device);
            if (err != ESP_OK) {
                ocs_loge(log_tag, "i2c_master_bus_rm_device() %s", esp_err_to_name(err));
            }
        }
    });
}

MasterTransceiver::MasterTransceiver(i2c_master_bus_handle_t bus,
                                     DevicePtr device,
                                     Address address)
    : address_(address)
    , bus_(bus)
    , device_(device) {
}

status::StatusCode
MasterTransceiver::send(const uint8_t* buf, size_t size, system::Time timeout) {
    if (timeout > 0 && timeout < system::Duration::millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_transmit(device_.get(), buf, size,
                                         timeout / system::Duration::millisecond);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "i2c_master_transmit() failed: addr=0x%" PRIx16 " err=%s",
                 address_, esp_err_to_name(err));

        if (err == ESP_ERR_INVALID_ARG) {
            return status::StatusCode::InvalidArg;
        }
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode
MasterTransceiver::receive(uint8_t* buf, size_t size, system::Time timeout) {
    if (timeout > 0 && timeout < system::Duration::millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_receive(device_.get(), buf, size,
                                        timeout / system::Duration::millisecond);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "i2c_master_receive() failed: addr=0x%" PRIx16 " err=%s",
                 address_, esp_err_to_name(err));

        if (err == ESP_ERR_INVALID_ARG) {
            return status::StatusCode::InvalidArg;
        }
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode MasterTransceiver::send_receive(const uint8_t* wbuf,
                                                   size_t wsize,
                                                   uint8_t* rbuf,
                                                   size_t rsize,
                                                   system::Time timeout) {
    if (timeout > 0 && timeout < system::Duration::millisecond) {
        return status::StatusCode::InvalidArg;
    }

    const auto err = i2c_master_transmit_receive(device_.get(), wbuf, wsize, rbuf, rsize,
                                                 timeout / system::Duration::millisecond);
    if (err != ESP_OK) {
        ocs_loge(log_tag,
                 "i2c_master_transmit_receive() failed: addr=0x%" PRIx16 " err=%s",
                 address_, esp_err_to_name(err));

        if (err == ESP_ERR_INVALID_ARG) {
            return status::StatusCode::InvalidArg;
        }
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace i2c
} // namespace io
} // namespace ocs
