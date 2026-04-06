/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/i2c/target_esp32/master_transceiver.h"

namespace ocs {
namespace io {
namespace i2c {

namespace {

const char* log_tag = "i2c_master_transceiver";

} // namespace

MasterTransceiver::Ptr
MasterTransceiver::make_device_ptr(i2c_master_dev_handle_t handle) {
    configASSERT(handle);

    return Ptr(handle, [](i2c_master_dev_handle_t h) {
        const auto err = i2c_master_bus_rm_device(h);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "i2c_master_bus_rm_device() %s", esp_err_to_name(err));
        }
    });
}

MasterTransceiver::MasterTransceiver(i2c_master_bus_handle_t bus,
                                     Ptr ptr,
                                     Address address)
    : address_(address)
    , bus_(bus)
    , ptr_(ptr) {
}

status::StatusCode
MasterTransceiver::send(const uint8_t* buf, size_t size, system::Time timeout) {
    if (timeout > 0) {
        if (timeout < system::Duration::millisecond) {
            return status::StatusCode::InvalidArg;
        }

        timeout /= system::Duration::millisecond;
    }

    const auto err = i2c_master_transmit(ptr_.get(), buf, size, timeout);
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
    if (timeout > 0) {
        if (timeout < system::Duration::millisecond) {
            return status::StatusCode::InvalidArg;
        }

        timeout /= system::Duration::millisecond;
    }

    const auto err = i2c_master_receive(ptr_.get(), buf, size, timeout);
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
    if (timeout > 0) {
        if (timeout < system::Duration::millisecond) {
            return status::StatusCode::InvalidArg;
        }

        timeout /= system::Duration::millisecond;
    }

    const auto err =
        i2c_master_transmit_receive(ptr_.get(), wbuf, wsize, rbuf, rsize, timeout);
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

status::StatusCode MasterTransceiver::probe(system::Time timeout) {
    if (timeout > 0) {
        if (timeout < system::Duration::millisecond) {
            return status::StatusCode::InvalidArg;
        }

        timeout /= system::Duration::millisecond;
    }

    const auto err = i2c_master_probe(bus_, address_, timeout);
    if (err != ESP_OK) {
        if (err == ESP_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        ocs_loge(log_tag, "i2c_master_probe(): addr=0x%" PRIx16 " err=%s", address_,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace i2c
} // namespace io
} // namespace ocs
