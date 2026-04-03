/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_heap_caps.h"

#include "ocs_system/target_esp32/heap_caps_arena.h"

namespace ocs {
namespace system {

HeapCapsArena::HeapCapsArena(uint32_t caps)
    : caps_(caps) {
}

void* HeapCapsArena::allocate(size_t size) {
    return heap_caps_malloc(size, caps_);
}

void HeapCapsArena::deallocate(void* ptr) {
    heap_caps_free(ptr);
}

} // namespace system
} // namespace ocs
