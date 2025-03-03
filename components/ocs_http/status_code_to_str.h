/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
