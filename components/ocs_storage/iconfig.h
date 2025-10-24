/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
