/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>

#include "ocs_status/code.h"

namespace ocs {
namespace storage {

class IStorage {
public:
    //! Destroy.
    virtual ~IStorage() = default;

    //! Read the size of the value for @p key.
    virtual status::StatusCode probe(const char* key, size_t& size) = 0;

    //! Read a key-value pair.
    //!
    //! @params
    //!  - @p key - name of value to read, 15 characters is a maximum length;
    //!  - @p value - value to read;
    //!  - @p size  - size of the value to read, in bytes.
    virtual status::StatusCode read(const char* key, void* value, size_t size) = 0;

    //! Write a key-value pair.
    //!
    //! @params
    //!  - @p key - name of value to write, 15 characters is a maximum length;
    //!  - @p value - value to write;
    //!  - @p size  - size of the value to write, in bytes.
    virtual status::StatusCode write(const char* key, const void* value, size_t size) = 0;

    //! Erase a key-value pair with the given key name.
    //!
    //! @params
    //!  - @p key - name of value to erase, 15 characters is a maximum length.
    virtual status::StatusCode erase(const char* key) = 0;

    //! Erase all key-value pairs.
    virtual status::StatusCode erase_all() = 0;
};

} // namespace storage
} // namespace ocs
