/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_persistent_counter.h"

namespace ocs {
namespace diagnostic {

class AccPersistentCounter : public BasicPersistentCounter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist counter values when reboot is happened.
    //!  - @p counter to handle actual counting value.
    AccPersistentCounter(storage::IStorage& storage, ICounter& counter);

    //! Returns the accumulated counter value, considering the previous persistent value.
    Value get() const override;
};

} // namespace diagnostic
} // namespace ocs
