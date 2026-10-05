/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_storage/async_storage.h"
#include "ocs_core/lock_guard.h"

namespace ocs {
namespace storage {

AsyncStorage::AsyncStorage(system::IArena& arena,
                           scheduler::IFuncScheduler& func_scheduler,
                           IStorage& storage,
                           TickType_t wait)
    : wait_(wait)
    , arena_(arena)
    , func_scheduler_(func_scheduler)
    , storage_(storage) {
    configASSERT(wait_);
}

status::StatusCode AsyncStorage::probe(const char* key, size_t& size) {
    auto sync_flag = system::make_shared_ptr<bool>(arena_, true);
    if (!sync_flag) {
        return status::StatusCode::NoMem;
    }

    return schedule_(sync_flag, [this, sync_flag, key, &size]() {
        core::LockGuard lock(mu_);

        return *sync_flag ? storage_.probe(key, size) : status::StatusCode::InvalidState;
    });
}

status::StatusCode AsyncStorage::read(const char* key, void* value, size_t size) {
    auto sync_flag = system::make_shared_ptr<bool>(arena_, true);
    if (!sync_flag) {
        return status::StatusCode::NoMem;
    }

    return schedule_(sync_flag, [this, sync_flag, key, value, size]() {
        core::LockGuard lock(mu_);

        return *sync_flag ? storage_.read(key, value, size)
                          : status::StatusCode::InvalidState;
    });
}

status::StatusCode AsyncStorage::write(const char* key, const void* value, size_t size) {
    auto sync_flag = system::make_shared_ptr<bool>(arena_, true);
    if (!sync_flag) {
        return status::StatusCode::NoMem;
    }

    return schedule_(sync_flag, [this, sync_flag, key, value, size]() {
        core::LockGuard lock(mu_);

        return *sync_flag ? storage_.write(key, value, size)
                          : status::StatusCode::InvalidState;
    });
}

status::StatusCode AsyncStorage::erase(const char* key) {
    auto sync_flag = system::make_shared_ptr<bool>(arena_, true);
    if (!sync_flag) {
        return status::StatusCode::NoMem;
    }

    return schedule_(sync_flag, [this, sync_flag, key]() {
        core::LockGuard lock(mu_);

        return *sync_flag ? storage_.erase(key) : status::StatusCode::InvalidState;
    });
}

status::StatusCode AsyncStorage::erase_all() {
    auto sync_flag = system::make_shared_ptr<bool>(arena_, true);
    if (!sync_flag) {
        return status::StatusCode::NoMem;
    }

    return schedule_(sync_flag, [this, sync_flag]() {
        core::LockGuard lock(mu_);

        return *sync_flag ? storage_.erase_all() : status::StatusCode::InvalidState;
    });
}

status::StatusCode AsyncStorage::schedule_(SyncFlag sync_flag,
                                           scheduler::IFuncScheduler::Func func) {
    auto future = func_scheduler_.add(func);
    if (!future) {
        return status::StatusCode::InvalidState;
    }

    auto code = future->wait(wait_);
    if (code == status::StatusCode::OK) {
        code = future->code();
    }

    core::LockGuard lock(mu_);

    *sync_flag = false;

    return code;
}

} // namespace storage
} // namespace ocs
