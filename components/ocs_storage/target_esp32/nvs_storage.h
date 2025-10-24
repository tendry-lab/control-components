/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <utility>

#include "nvs.h"
#include "nvs_flash.h"

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace storage {

class NvsStorage : public IStorage, private core::NonCopyable<> {
public:
    //! Maximum number of symbols for the storage namespace, without NULL character.
    static constexpr unsigned max_namespace_len = NVS_KEY_NAME_MAX_SIZE - 1;

    //! Initialize.
    //!
    //! @params
    //!  - @p ns - NVS namespace.
    //!
    //! @remarks
    //!  - NVS should be initialized.
    //!  - namespace length should not exceed @p max_namespace_len.
    explicit NvsStorage(const char* ns);

    //! Read data size from the configured namespace.
    status::StatusCode probe(const char* key, size_t& size) override;

    //! Read data from the configured namespace.
    status::StatusCode read(const char* key, void* value, size_t size) override;

    //! Write data to the configured namespace.
    status::StatusCode write(const char* key, const void* value, size_t size) override;

    //! Erase data from the configured namespace.
    status::StatusCode erase(const char* key) override;

    //! Erase all data from the configured namespace.
    status::StatusCode erase_all() override;

private:
    std::pair<nvs_handle_t, status::StatusCode> open_(nvs_open_mode_t mode);

    status::StatusCode
    read_(nvs_handle_t handle, const char* key, void* value, size_t& size);

    status::StatusCode
    write_(nvs_handle_t handle, const char* key, const void* value, size_t size);

    status::StatusCode erase_(nvs_handle_t handle, const char* key);
    status::StatusCode erase_all_(nvs_handle_t handle);

    char ns_[max_namespace_len + 1];
};

} // namespace storage
} // namespace ocs
