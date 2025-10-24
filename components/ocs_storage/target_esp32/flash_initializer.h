/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace storage {

class FlashInitializer : private core::NonCopyable<> {
public:
    //! Initialize NVS.
    FlashInitializer();

    //! Deinitialize NVS.
    ~FlashInitializer();
};

} // namespace storage
} // namespace ocs
