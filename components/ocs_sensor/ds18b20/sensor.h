/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/macros.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_onewire/bus.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_scheduler/itask.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

class Sensor : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Various sensor configuration.
    struct OCS_ATTR_PACKED Configuration {
        //! Resolution Bits | Max Conversation Time |
        //! --------------- | --------------------- |
        //! 12              | 750.0ms (tconv)       |
        //! 11              | 375.0ms (tconv / 2)   |
        //! 10              | 187.5ms (tconv / 4)   |
        //! 9               | 93.75ms (tconv / 8)   |
        enum class Resolution : uint8_t {
            None,
            Bit_12,
            Bit_11,
            Bit_10,
            Bit_9,
        };

        bool operator==(const Configuration&) const;
        bool operator!=(const Configuration&) const;

        onewire::RomCode rom_code;
        Resolution resolution { Resolution::None };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist sensor configuration.
    //!  - @p id - unique sensor ID, to distinguish one sensor from another.
    Sensor(storage::IStorage& storage, const char* id);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return true if the sensor was assigned to one of the 1-Wire buses.
    bool configured() const;

    //! Return human readable sensor identifier.
    const char* id() const;

    //! Return the latest sensor data.
    float get_data() const;

    //! Read sensor configuration from persistent storage.
    status::StatusCode read_configuration(Configuration& configuration);

    //! Write sensor configuration.
    //!
    //! @notes
    //!  @p configuration is kept in non-volatile storage until the next
    //!  write_configuration() call. @p bus is assigned to the sensor, making
    //!  the sensor ready to send temperature data to the outside world.
    status::StatusCode write_configuration(onewire::Bus& bus,
                                           const Configuration& configuration);

    //! Erase sensor configuration.
    //!
    //! @params
    //!  - @p configuration to store erased configuration.
    //!
    //! @notes
    //!  - Remove sensor persistent configuration.
    //!  - De-assign sensor from the bus.
    status::StatusCode erase_configuration(Configuration& configuration);

private:
    const std::string id_;

    storage::IStorage& storage_;

    onewire::Bus* bus_ { nullptr };
    Configuration configuration_;
    core::SpmcNode<float> data_;
};

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
