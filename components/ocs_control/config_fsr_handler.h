/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "ocs_control/ifsr_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_storage/iconfig.h"

namespace ocs {
namespace control {

class ConfigFsrHandler : public IFsrHandler, private core::NonCopyable<> {
public:
    //! Reset added configs on the FSR event.
    status::StatusCode handle_fsr() override;

    //! Add @p config to be reset when the FSR is happened.
    void add(storage::IConfig& config);

private:
    std::vector<storage::IConfig*> configs_;
};

} // namespace control
} // namespace ocs
