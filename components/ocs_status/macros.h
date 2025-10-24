/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_compiler.h"

#include "ocs_status/code.h"

//! If the status code is not OK, it returns.
#define OCS_STATUS_RETURN_ON_ERROR(x)                                                    \
    do {                                                                                 \
        const ocs::status::StatusCode _code = (x);                                       \
        if (unlikely(_code != ocs::status::StatusCode::OK)) {                            \
            return _code;                                                                \
        }                                                                                \
    } while (0)

//! If the condition isn't true, the code is returned.
#define OCS_STATUS_RETURN_ON_FALSE(x, code)                                              \
    do {                                                                                 \
        if (unlikely(!(x))) {                                                            \
            return code;                                                                 \
        }                                                                                \
    } while (0)
