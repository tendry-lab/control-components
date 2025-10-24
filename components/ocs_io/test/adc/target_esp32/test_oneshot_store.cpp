/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <vector>

#include "soc/soc_caps.h"
#include "unity.h"

#include "ocs_io/adc/target_esp32/line_fitting_converter.h"
#include "ocs_io/adc/target_esp32/oneshot_store.h"

namespace ocs {
namespace io {
namespace adc {

TEST_CASE("Oneshot ADC store: register maximum number of ADC",
          "[ocs_io], [adc_oneshot_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        OneshotStore store(unit, ADC_ATTEN_DB_12, ADC_BITWIDTH_10);

        const unsigned count = SOC_ADC_CHANNEL_NUM(unit);

        for (unsigned n = 0; n < count; ++n) {
            const Channel channel = static_cast<Channel>(n);
            TEST_ASSERT_NOT_NULL(store.add(channel));
        }
    }
}

TEST_CASE("Oneshot ADC store: register the same ADC twice",
          "[ocs_io], [adc_oneshot_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        OneshotStore store(unit, ADC_ATTEN_DB_12, ADC_BITWIDTH_10);

        const Channel channel = ADC_CHANNEL_5;
        TEST_ASSERT_NOT_NULL(store.add(channel));
        TEST_ASSERT_NULL(store.add(channel));
    }
}

TEST_CASE("Oneshot ADC store: register overflow", "[ocs_io], [adc_oneshot_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        OneshotStore store(unit, ADC_ATTEN_DB_12, ADC_BITWIDTH_10);

        const unsigned count = SOC_ADC_CHANNEL_NUM(unit);

        for (unsigned n = 0; n < count; ++n) {
            const Channel channel = static_cast<Channel>(n);
            TEST_ASSERT_NOT_NULL(store.add(channel));
        }

        const Channel channel = static_cast<Channel>(count);
        TEST_ASSERT_NULL(store.add(channel));
    }
}

TEST_CASE("Oneshot ADC store: read/convert operations", "[ocs_io], [adc_oneshot_store]") {
    const std::vector<adc_unit_t> units {
        ADC_UNIT_1,
        ADC_UNIT_2,
    };

    for (const auto& unit : units) {
        OneshotStore store(unit, ADC_ATTEN_DB_12, ADC_BITWIDTH_10);
        LineFittingConverter converter(unit, ADC_ATTEN_DB_12, ADC_BITWIDTH_10);

        const Channel channel = ADC_CHANNEL_5;

        auto reader = store.add(channel);
        TEST_ASSERT_NOT_NULL(reader);

        int raw = 0;
        TEST_ASSERT_EQUAL(status::StatusCode::OK, reader->read(raw));

        int voltage = 0;
        TEST_ASSERT_EQUAL(status::StatusCode::OK, converter.convert(voltage, raw));
    }
}

} // namespace adc
} // namespace io
} // namespace ocs
