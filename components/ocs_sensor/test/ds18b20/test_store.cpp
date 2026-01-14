/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_io/gpio/types.h"
#include "ocs_sensor/ds18b20/sensor.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_test/memory_storage.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

struct TestDelayer : public system::IRtDelayer, private core::NonCopyable<> {
    status::StatusCode delay(system::Time delay) {
        if (delay < system::Duration::millisecond) {
            return status::StatusCode::InvalidArg;
        }

        vTaskDelay(pdMS_TO_TICKS(delay / system::Duration::millisecond));

        return status::StatusCode::OK;
    }
};

} // namespace

TEST_CASE("DS18B20 store: schedule: empty store", "[ocs_sensor], [ds18b20_store]") {
    const io::gpio::GpioNum gpio_num = GPIO_NUM_26;

    TestDelayer delayer;
    Store store(delayer, 16);

    auto future =
        store.schedule(gpio_num, [](onewire::Bus& bus, Store::SensorList& sensors) {
            return status::StatusCode::OK;
        });
    TEST_ASSERT_NULL(future);
}

TEST_CASE("DS18B20 store: schedule: invalid GPIO", "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const io::gpio::GpioNum gpio_num = GPIO_NUM_26;
    const io::gpio::GpioNum invalid_gpio_num = GPIO_NUM_27;

    TEST_ASSERT_NOT_EQUAL(gpio_num, invalid_gpio_num);

    TestDelayer delayer;
    Store store(delayer, 16);
    test::MemoryStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio_num));

    auto future = store.schedule(invalid_gpio_num,
                                 [](onewire::Bus& bus, Store::SensorList& sensors) {
                                     return status::StatusCode::OK;
                                 });
    TEST_ASSERT_NULL(future);
}

TEST_CASE("DS18B20 store: add sensor", "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const io::gpio::GpioNum gpio_num = GPIO_NUM_26;

    TestDelayer delayer;
    Store store(delayer, 16);
    test::MemoryStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio_num));

    auto future = store.schedule(
        gpio_num, [&sensor](onewire::Bus& bus, Store::SensorList& sensors) {
            TEST_ASSERT_EQUAL(1, sensors.size());
            TEST_ASSERT_EQUAL_STRING(sensor.id(), sensors[0]->id());
            return status::StatusCode::OK;
        });
    TEST_ASSERT_NOT_NULL(future);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait(pdMS_TO_TICKS(1000 * 5)));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());
}

TEST_CASE("DS18B20 store: read sensor configuration: non-configured",
          "[ocs_sensor], [ds18b20_store]") {
    const char* sensor_id = "test_sensor";
    const io::gpio::GpioNum gpio_num = GPIO_NUM_26;

    TestDelayer delayer;
    Store store(delayer, 16);
    test::MemoryStorage storage;
    Sensor sensor(storage, sensor_id);

    TEST_ASSERT_FALSE(sensor.configured());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, store.add(sensor, gpio_num));
    TEST_ASSERT_FALSE(sensor.configured());

    auto future =
        store.schedule(gpio_num, [](onewire::Bus& bus, Store::SensorList& sensors) {
            TEST_ASSERT_EQUAL(1, sensors.size());

            TEST_ASSERT_FALSE(sensors[0]->configured());

            Sensor::Configuration configuration;
            TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                              sensors[0]->read_configuration(configuration));

            return status::StatusCode::OK;
        });
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
