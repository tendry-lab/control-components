/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "ocs_core/byte_writer.h"

namespace ocs {
namespace core {

ByteWriter::ByteWriter(uint8_t* data, size_t size)
    : size_(size)
    , data_(data) {
}

size_t ByteWriter::get_len() const {
    return offset_;
}

size_t ByteWriter::get_cap() const {
    return size_;
}

uint8_t* ByteWriter::get_data() {
    return data_;
}

size_t ByteWriter::write(const uint8_t* data, size_t size) {
    const size_t ret = std::min(size, left_());

    if (ret) {
        memcpy(data_ + offset_, data, ret);
        offset_ += ret;
    }

    return ret;
}

bool ByteWriter::write_byte(uint8_t data) {
    return write(data);
}

bool ByteWriter::write_word(uint16_t data) {
    return write(data);
}

size_t ByteWriter::reserve(size_t size) {
    const size_t ret = std::min(size, left_());
    offset_ += ret;

    return ret;
}

size_t ByteWriter::left_() const {
    return size_ - offset_;
}

} // namespace core
} // namespace ocs
