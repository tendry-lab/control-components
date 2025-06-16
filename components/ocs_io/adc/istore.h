/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
