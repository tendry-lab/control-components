/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "ocs_core/istream_reader.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class FileStreamReader : public IStreamReader, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p path - file path.
    explicit FileStreamReader(const char* path);

    //! Ensure the file reading is finished.
    ~FileStreamReader();

    //! Begin file reading.
    status::StatusCode begin() override;

    //! End file reading.
    //!
    //! @remarks
    //!  Ensure that file reading is ended to prevent file descriptor leakage.
    status::StatusCode end() override;

    //! Start file reading from the beginning of the file.
    status::StatusCode cancel() override;

    //! Read up to @p size bytes from file to @p data.
    status::StatusCode read(void* data, unsigned& size) override;

private:
    const std::string path_;

    FILE* file_ { nullptr };
};

} // namespace core
} // namespace ocs
