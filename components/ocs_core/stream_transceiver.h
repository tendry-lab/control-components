/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <vector>

#include "ocs_core/istream_reader.h"
#include "ocs_core/istream_writer.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class StreamTransceiver : private core::NonCopyable<> {
public:
    using Buffer = std::vector<uint8_t>;

    //! Initialize.
    //!
    //! @params
    //!  - @p reader to read data from.
    //!  - @p writer to write data to.
    //!  - @p buffer to store the data.
    StreamTransceiver(IStreamReader& reader, IStreamWriter& writer, Buffer& buffer);

    //! Automatically end stream transferring.
    ~StreamTransceiver();

    //! Transceive data from reader to writer.
    status::StatusCode transceive();

private:
    IStreamReader& reader_;
    IStreamWriter& writer_;
    Buffer buffer_;
};

} // namespace core
} // namespace ocs
