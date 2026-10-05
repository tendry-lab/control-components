/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_scheduler/ifunc_scheduler.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace storage {

class AsyncStorage : public IStorage, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p func_scheduler to schedule asynchronous storage operations.
    //!  - @p storage to handle the storage operation in the asynchronous context.
    //!  - @p wait - how long to wait for the asynchronous operations to finish.
    AsyncStorage(system::IArena& arena,
                 scheduler::IFuncScheduler& func_scheduler,
                 IStorage& storage,
                 TickType_t wait);

    //! Read the size of the value for @p key.
    status::StatusCode probe(const char* key, size_t& size) override;

    //! Read a key-value pair.
    status::StatusCode read(const char* key, void* value, size_t size) override;

    //! Write a key-value pair.
    status::StatusCode write(const char* key, const void* value, size_t size) override;

    //! Erase a key-value pair with the given key name.
    status::StatusCode erase(const char* key) override;

    //! Erase all key-value pairs.
    status::StatusCode erase_all() override;

private:
    using SyncFlag = std::shared_ptr<bool>;

    status::StatusCode schedule_(SyncFlag sync_flag,
                                 scheduler::IFuncScheduler::Func func);

    const TickType_t wait_ { 0 };

    system::IArena& arena_;
    scheduler::IFuncScheduler& func_scheduler_;
    IStorage& storage_;

    core::StaticMutex mu_;
};

} // namespace storage
} // namespace ocs
