/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

status::StatusCode ChunkStreamWriter::write(const void* data, unsigned size) {
    return writer_.write(data, size);
}

} // namespace http
} // namespace ocs
