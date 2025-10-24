/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_http/ihandler.h"

namespace ocs {
namespace http {

//! Pattern iterator.
class IPatternIterator {
public:
    //! Destroy.
    virtual ~IPatternIterator() = default;

    //! Handle pattern.
    virtual void iterate_pattern(const char* pattern, IHandler& handler) = 0;
};

} // namespace http
} // namespace ocs
