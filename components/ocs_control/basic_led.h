/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/iled.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace control {

class BasicLED : public ILED, private core::NonCopyable<BasicLED> {
public:
    //! Destroy.
    virtual ~BasicLED() = default;

    //! Mark LED as in use.
    status::StatusCode try_lock(Priority priority) override;

    //! Mark LED as free to use.
    status::StatusCode try_unlock(Priority priority) override;

protected:
    bool inuse_ { false };

private:
    Priority priority_ { 0 };
};

} // namespace control
} // namespace ocs
