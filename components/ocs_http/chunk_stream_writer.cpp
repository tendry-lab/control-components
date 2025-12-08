/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_http/chunk_stream_writer.h"

namespace ocs {
namespace http {

ChunkStreamWriter::ChunkStreamWriter(IResponseWriter& writer)
    : writer_(writer) {
}

status::StatusCode ChunkStreamWriter::begin() {
    return status::StatusCode::OK;
}

status::StatusCode ChunkStreamWriter::end() {
    return writer_.write(nullptr, 0);
}

status::StatusCode ChunkStreamWriter::cancel() {
    return end();
}

status::StatusCode ChunkStreamWriter::write(const void* data, size_t size) {
    return writer_.write(data, size);
}

} // namespace http
} // namespace ocs
