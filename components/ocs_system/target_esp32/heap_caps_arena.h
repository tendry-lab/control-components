/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace system {

class HeapCapsArena : public IArena, private ocs::core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p caps - capabilities of the various memory systems. See "esp_heap_caps.h".
    explicit HeapCapsArena(uint32_t caps);

    //! Allocate a chunk of memory which has the given capabilities.
    void* allocate(size_t size) override;

    //! Free memory previously allocated memory.
    void deallocate(void* ptr) override;

private:
    const uint32_t caps_ { 0 };
};

} // namespace system
} // namespace ocs
