/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/spi/target_esp32/master_store.h"
#include "ocs_io/spi/target_esp32/master_transceiver.h"

namespace ocs {
namespace io {
namespace spi {

namespace {

const char* log_tag = "spi_master_store";

} // namespace

MasterStore::MasterStore(MasterStore::Params params)
    : params_(params) {
    configASSERT(params.max_transfer_size);

    spi_bus_config_t config;
    memset(&config, 0, sizeof(config));

    config.mosi_io_num = params_.mosi;
    config.miso_io_num = params_.miso;
    config.sclk_io_num = params_.sclk;
    config.quadwp_io_num = -1;
    config.quadhd_io_num = -1;
    config.max_transfer_sz = params_.max_transfer_size;

    ESP_ERROR_CHECK(spi_bus_initialize(static_cast<spi_host_device_t>(params_.host_id),
                                       &config, SPI_DMA_DISABLED));
}

IStore::ITransceiverPtr
MasterStore::add(const char* id, gpio::Gpio cs, Mode mode, TransferSpeed speed) {
    configASSERT(id);
    configASSERT(cs != static_cast<io::gpio::Gpio>(-1));
    configASSERT(speed > 0);

    spi_device_interface_config_t config;
    memset(&config, 0, sizeof(config));

    config.clock_speed_hz = speed;
    config.spics_io_num = cs;
    config.queue_size = 1;
    config.mode = mode;

    spi_device_handle_t device = nullptr;

    auto err = spi_bus_add_device(static_cast<spi_host_device_t>(params_.host_id),
                                  &config, &device);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "spi_bus_add_device() failed: id=%s err=%s", id,
                 esp_err_to_name(err));

        return nullptr;
    }

    auto device_ptr = MasterTransceiver::make_device_shared(device);
    configASSERT(device_ptr);

    return IStore::ITransceiverPtr(new (std::nothrow) MasterTransceiver(device_ptr, id));
}

MasterStore::~MasterStore() {
    ESP_ERROR_CHECK(spi_bus_free(static_cast<spi_host_device_t>(params_.host_id)));
}

} // namespace spi
} // namespace io
} // namespace ocs
