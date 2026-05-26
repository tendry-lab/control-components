/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <atomic>

#include "ocs_core/noncopyable.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace system {

class HeapArena : public IArena, private ocs::core::NonCopyable<> {
public:
    //! Allocate a chunk of memory via malloc().
    void* allocate(size_t size) override;

    //! Free memory previously allocated memory via free().
    void deallocate(void* ptr) override;
};

} // namespace system
} // namespace ocs
