/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_io/adc/ireader.h"
#include "ocs_io/adc/types.h"

namespace ocs {
namespace io {
namespace adc {

class IStore {
public:
    using IReaderPtr = std::shared_ptr<IReader>;

    //! Destroy.
    virtual ~IStore() = default;

    //! Configure ADC reading for @p channel.
    virtual IReaderPtr add(Channel channel) = 0;
};

} // namespace adc
} // namespace io
} // namespace ocs
