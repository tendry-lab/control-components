/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace core {

//! Base class for non-copyable objects.
//!
//! @note
//!  - Template allows instantiate distinct non-copyable bases when
//!    they are inherited by multiple paths. We need this to
//!    eliminate compiler warnings "inaccessible direct base...".
//!
//!  - If any of the copy operations is declared, then none of the move operation
//!    will be generated, thus non-copyable object will become non-movable. It
//!    can become moveable only when move operations are declared explicitly.
template <class T = void> class NonCopyable {
protected:
    virtual ~NonCopyable() = default;
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

} // namespace core
} // namespace ocs
