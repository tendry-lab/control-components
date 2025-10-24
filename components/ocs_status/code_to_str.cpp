/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_status/code_to_str.h"

namespace ocs {
namespace status {

const char* code_to_str(StatusCode status) {
    switch (status) {
    case StatusCode::OK:
        return "OK";
    case StatusCode::Error:
        return "Error";
    case StatusCode::NoData:
        return "NoData";
    case StatusCode::NoMem:
        return "NoMem";
    case StatusCode::IsrYield:
        return "IsrYield";
    case StatusCode::InvalidArg:
        return "InvalidArg";
    case StatusCode::InvalidState:
        return "InvalidState";
    case StatusCode::Timeout:
        return "Timeout";
    case StatusCode::NotModified:
        return "NotModified";
    case StatusCode::NotSupported:
        return "NotSupported";

    case StatusCode::Last:
        break;
    }

    return "<none>";
}

} // namespace status
} // namespace ocs
