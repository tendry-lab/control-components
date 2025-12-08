/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_core/log.h"
#include "ocs_io/spi/target_esp32/master_transceiver.h"

namespace ocs {
namespace io {
namespace spi {

namespace {

const char* log_tag = "spi_master_transceiver";

} // namespace

MasterTransceiver::DevicePtr MasterTransceiver::make_device_shared(spi_device_t* device) {
    return MasterTransceiver::DevicePtr(device, [](spi_device_t* device) {
        if (device) {
            const auto err = spi_bus_remove_device(device);
            if (err != ESP_OK) {
                ocs_loge(log_tag, "spi_bus_remove_device() %s", esp_err_to_name(err));
            }
        }
    });
}

MasterTransceiver::MasterTransceiver(MasterTransceiver::DevicePtr device, const char* id)
    : id_(id)
    , device_(device) {
}

status::StatusCode MasterTransceiver::transceive(const uint8_t* send_buf,
                                                 size_t send_buf_size,
                                                 uint8_t* recv_buf,
                                                 size_t recv_buf_size) {
    spi_transaction_t transaction;
    memset(&transaction, 0, sizeof(transaction));

    transaction.tx_buffer = send_buf;
    transaction.length = send_buf_size * 8;
    transaction.rx_buffer = recv_buf;
    transaction.rxlength = recv_buf_size * 8;

    const auto err = spi_device_transmit(device_.get(), &transaction);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "spi_device_transmit() failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace spi
} // namespace io
} // namespace ocs
