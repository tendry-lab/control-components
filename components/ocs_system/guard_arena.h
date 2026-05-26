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

class GuardArena : public IArena, private ocs::core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform actual allocations/deallocations.
    explicit GuardArena(IArena& arena);

    //! Check for the memory leaks.
    ~GuardArena();

    //! Allocate memory, count each allocation.
    void* allocate(size_t size) override;

    //! Free memory previously allocated memory, count each deallocation.
    void deallocate(void* ptr) override;

    //! Return current number of allocations.
    size_t get_num_allocations() const;

private:
    IArena& arena_;

    std::atomic<size_t> num_allocations_ { 0 };
};

} // namespace system
} // namespace ocs
