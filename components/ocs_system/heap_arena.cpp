/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdlib>

#include "ocs_system/heap_arena.h"

namespace ocs {
namespace system {

void* HeapArena::allocate(size_t size) {
    return malloc(size);
}

void HeapArena::deallocate(void* ptr) {
    free(ptr);
}

} // namespace system
} // namespace ocs
