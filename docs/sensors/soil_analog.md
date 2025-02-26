## Introduction

Analog soil moisture sensor allows to monitor the following soil characteristics:
- Current soil status: Error/Dry/Depletion/Wet/Saturated
- Current soil status duration, in seconds
- Previous soil status: Error/Dry/Depletion/Wet/Saturated
- Previous soil status duration, in seconds
- Current soil status status progress, 0 - beginning of the status, 100 - end of the status.
- Soil status statistics, written to the flash once per configured interval (30min - 2h)

It's required to calibrate the sensor before first usage.

## Calibration

During the calibration process it's required to monitor the value of the "raw" field of the calibrating sensor ("sensor_soil_raw") in the device telemetry data.

The calibration procedure includes the following steps:
- Measure the value from the sensor in the air. It's the max value.
- Measure the value from the sensor in the water. It's the min value.

## HTTP API

**Get configuration for all analog sensors**

http "bonsai-growlab.local/api/v1/config/sensor/analog"

```json
[
    {
        "bitwidth": 10,
        "id": "ldr_a0",
        "max": 950,
        "min": 50,
        "oversampling": 32
    },
    {
        "bitwidth": 10,
        "id": "soil_a0",
        "max": 540,
        "min": 240,
        "oversampling": 32
    }
]
```

**Get soil sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog?id=soil_a0"

```json
{
    "bitwidth": 10,
    "max": 540,
    "min": 240,
    "oversampling": 32
}
```

**Set soil sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog?id=soil_a0&min=200&max=800&oversampling=32"

```txt
OK
```

- id - unique sensor identifier.
- max - maximum possible sensor value.
- min - minimum possible sensor value.
- oversampling - number of times a sensor value should be measured.

**Reset soil sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog/id=soil_a0&reset=1"

```txt
OK
```

- id - unique sensor identifier.

## Tested Sensors

- YL-69 Soil Moisture Sensor
- Capacitive Soil Moisture Sensor V1.2. [Datasheet](https://www.datocms-assets.com/28969/1662716326-hw-101-hw-moisture-sensor-v1-0.pdf)

## Firmware Configuration Options

- CONFIG_BONSAI_FIRMWARE_SENSOR_SOIL_ANALOG_ADC_CHANNEL
- CONFIG_BONSAI_FIRMWARE_SENSOR_SOIL_ANALOG_VALUE_MAX
- CONFIG_BONSAI_FIRMWARE_SENSOR_SOIL_ANALOG_VALUE_MIN
- CONFIG_BONSAI_FIRMWARE_SENSOR_SOIL_ANALOG_READ_INTERVAL
