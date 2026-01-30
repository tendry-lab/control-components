/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "ocs_core/byte_reader.h"

namespace ocs {
namespace core {

ByteReader::ByteReader(const uint8_t* data, size_t size)
    : data_(data)
    , size_(size) {
}

size_t ByteReader::get_len() const {
    return size_ - offset_;
}

size_t ByteReader::get_cap() const {
    return size_;
}

const uint8_t* ByteReader::get_data() const {
    return get_len() ? data_ + offset_ : nullptr;
}

size_t ByteReader::read(uint8_t* data, size_t size) {
    const size_t ret = std::min(size, get_len());

    if (ret) {
        memcpy(data, get_data(), ret);
        offset_ += ret;
    }

    return ret;
}

} // namespace core
} // namespace ocs
