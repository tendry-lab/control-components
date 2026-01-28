/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class ByteReader : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @remarks
    //!  - @p data should be valid until ByteReader exists.
    ByteReader(const uint8_t* data, size_t size);

    //! Return number of bytes left.
    size_t get_len() const;

    //! Return size of the underlying array of bytes.
    size_t get_cap() const;

    //! Return pointer to the underlying data.
    //!
    //! @remarks
    //!  Offset is taken into account.
    const uint8_t* get_data() const;

    //! Read @p size bytes to @p data buffer.
    //!
    //! @returns
    //!  Number of read bytes or 0 if no bytes left.
    size_t read(uint8_t* data, size_t size);

    //! Read any integral value.
    template <typename T> bool read(T& t) {
        static_assert(std::is_integral<T>::value, "require integer type");

        if (get_len() < sizeof(t)) {
            return false;
        }

        t = 0;

        for (size_t n = offset_; n < offset_ + sizeof(t); ++n) {
            t = (t << 8) | data_[n];
        }

        offset_ += sizeof(t);

        return true;
    }

private:
    const uint8_t* data_ { nullptr };
    const size_t size_ { 0 };

    size_t offset_ { 0 };
};

} // namespace core
} // namespace ocs
