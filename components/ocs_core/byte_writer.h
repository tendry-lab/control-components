/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <type_traits>

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class ByteWriter : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  - @p data should be valid until ByteWriter exists.
    ByteWriter(uint8_t* data, size_t size);

    //! Return the number of written bytes.
    size_t get_len() const;

    //! Return the size of the underlying array of bytes.
    size_t get_cap() const;

    //! Return the underlying data.
    uint8_t* get_data();

    //! Write @p size bytes of @p data.
    size_t write(const uint8_t* data, size_t size);

    //! Reserve space for @p size bytes.
    //!
    //! @return
    //!  Number of reserved bytes.
    size_t reserve(size_t size);

    //! Write any integer value.
    template <typename T> bool write(const T& t) {
        static_assert(std::is_integral<T>::value, "require integral type");

        if (left_() < sizeof(t)) {
            return false;
        }

        T c = t;

        for (size_t n = offset_ + sizeof(t); n > offset_;) {
            --n;
            data_[n] = (c & 0xFF);
            c >>= 8;
        }

        offset_ += sizeof(t);

        return true;
    }

private:
    size_t left_() const;

    const size_t size_ { 0 };

    uint8_t* data_ { nullptr };
    size_t offset_ { 0 };
};

} // namespace core
} // namespace ocs
