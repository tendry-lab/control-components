/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/spi_master.h"

namespace ocs {
namespace io {
namespace spi {

//! SPI mode, representing a pair of (CPOL, CPHA) configuration:
//! - 0: (0, 0)
//! - 1: (0, 1)
//! - 2: (1, 0)
//! - 3: (1, 1)
//!
//! @references
//!  https://en.wikipedia.org/wiki/Serial_Peripheral_Interface
//!  https://www.analog.com/en/resources/analog-dialogue/articles/introduction-to-spi-interface.html
using Mode = uint8_t;

//! SPI peripheral unique identifier.
using HostID = uint8_t;

//! SPI transfer speed, in hertz.
using TransferSpeed = int;

//! Virtual SPI peripheral.
const HostID VSPI = SPI3_HOST;

} // namespace spi
} // namespace io
} // namespace ocs
