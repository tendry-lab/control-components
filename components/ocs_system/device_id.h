/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_security/sha_to_hex_str.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace system {

class DeviceID : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p uuid - unique string identifier used during device ID generation.
    DeviceID(system::IArena& arena, const char* uuid);

    //! Return human-readable unique string device identifier.
    const char* get_id() const;

private:
    UniquePtr<security::sha_to_hex_str> id_;
};

} // namespace system
} // namespace ocs
