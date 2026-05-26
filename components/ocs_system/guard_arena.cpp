/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_system/guard_arena.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "guard_arena";

} // namespace

GuardArena::GuardArena(IArena& arena)
    : arena_(arena) {
}

GuardArena::~GuardArena() {
    if (num_allocations_) {
        ocs_loge(log_tag, "memory leaks detected: %zu allocations were not freed",
                 num_allocations_.load());
    }
    configASSERT(!num_allocations_);
}

void* GuardArena::allocate(size_t size) {
    void* ptr = arena_.allocate(size);
    if (!ptr) {
        return nullptr;
    }

    ++num_allocations_;

    return ptr;
}

void GuardArena::deallocate(void* ptr) {
    configASSERT(num_allocations_);
    --num_allocations_;

    arena_.deallocate(ptr);
}

size_t GuardArena::get_num_allocations() const {
    return num_allocations_;
}

} // namespace system
} // namespace ocs
