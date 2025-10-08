/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/button_event_handler.h"

namespace ocs {
namespace control {

ButtonEventHandler::ButtonEventHandler(system::IClock& clock,
                                       IButton& button,
                                       IButtonHandler& handler)
    : clock_(clock)
    , button_(button)
    , handler_(handler) {
}

status::StatusCode ButtonEventHandler::handle_event() {
    const auto changed = (was_pressed_ != button_.get());
    if (!changed) {
        return status::StatusCode::OK;
    }

    if (!pressed_ts_) {
        was_pressed_ = true;
        pressed_ts_ = clock_.now();

        return status::StatusCode::OK;
    }

    const system::Time duration = clock_.now() - pressed_ts_;

    pressed_ts_ = 0;
    was_pressed_ = false;

    return handler_.handle_pressed(duration);
}

} // namespace control
} // namespace ocs
