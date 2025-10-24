/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_control/ilocator.h"
#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class LocateHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @locator to control device locating.
    explicit LocateHandler(control::ILocator& locator);

    // Control device locating over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

private:
    control::ILocator& locator_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
