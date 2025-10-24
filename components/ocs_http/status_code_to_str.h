/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_http/status_code.h"

namespace ocs {
namespace http {

//! Return a text for the HTTP status code.
//!
//! @remarks
//!  Return the empty string if the code is unknown.
const char* status_code_to_str(StatusCode code);

} // namespace http
} // namespace ocs
