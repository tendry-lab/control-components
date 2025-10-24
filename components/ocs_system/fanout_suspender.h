/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_system/isuspend_handler.h"
#include "ocs_system/isuspender.h"

namespace ocs {
namespace system {

class FanoutSuspender : public ISuspender, private core::NonCopyable<> {
public:
    //! Suspend the system.
    status::StatusCode suspend() override;

    //! Resume the system.
    status::StatusCode resume() override;

    //! Add @p handler to be notified about suspend/resume events.
    //!
    //! @params
    //!  - @p handler to handle suspend/resume event.
    //!  - @p id to distinguish one handler from another.
    status::StatusCode add(ISuspendHandler& handler, const char* id);

private:
    class Node : public ISuspendHandler {
    public:
        Node(ISuspendHandler& handler, const char* id)
            : id_(id)
            , handler_(handler) {
        }

        status::StatusCode handle_suspend() override;
        status::StatusCode handle_resume() override;

        const char* id() const;

    private:
        const std::string id_;

        ISuspendHandler& handler_;
    };

    using NodeList = std::vector<Node>;

    NodeList nodes_;
};

} // namespace system
} // namespace ocs
