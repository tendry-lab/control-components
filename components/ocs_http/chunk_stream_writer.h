/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>

#include "ocs_core/istream_writer.h"
#include "ocs_core/noncopyable.h"
#include "ocs_http/iresponse_writer.h"

namespace ocs {
namespace http {

//! Use chunked-encoding and send the response in the form of chunks.
class ChunkStreamWriter : public core::IStreamWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p writer to write chunks of data.
    explicit ChunkStreamWriter(IResponseWriter& writer);

    //! Begin stream writing.
    status::StatusCode begin() override;

    //! End stream writing.
    status::StatusCode end() override;

    //! Cancel stream writing.
    status::StatusCode cancel() override;

    //! Write @p size bytes of @p data.
    status::StatusCode write(const void* data, size_t size) override;

private:
    IResponseWriter& writer_;
};

} // namespace http
} // namespace ocs
