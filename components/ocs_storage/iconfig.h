/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace storage {

class IConfig {
public:
    //! Destroy.
    virtual ~IConfig() = default;

    //! Reset configuration.
    virtual status::StatusCode reset() = 0;
};

} // namespace storage
} // namespace ocs
