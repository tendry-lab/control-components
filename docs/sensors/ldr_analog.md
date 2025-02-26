## Introduction

Analog LDR (light dependent resistor) sensor allows to measure the lightness of the environment.

It's required to calibrate the sensor before first usage.

## Calibration

During the calibration process it's required to monitor the value of the "raw" field of the calibrating sensor ("sensor_ldr_raw") in the device telemetry data.

The calibration procedure includes the following steps:
- Measure the value from the sensor in the full light environment. It's the max value.
- Measure the value from the sensor in an almost dark environment. It's the min value.

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

**Get LDR sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog?id=ldr_a0"

```json
{
    "bitwidth": 10,
    "max": 950,
    "min": 50,
    "oversampling": 32
}
```

**Set LDR sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog?id=ldr_a0&min=200&max=800&oversampling=32"

```txt
OK
```

- id - unique sensor identifier.
- max - maximum possible sensor value.
- min - minimum possible sensor value.
- oversampling - number of times a sensor value should be measured.

**Reset LDR sensor configuration**

http "bonsai-growlab.local/api/v1/config/sensor/analog/id=ldr_a0&reset=1"

```txt
OK
```

- id - unique sensor identifier.

## Firmware Configuration Options

- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_ADC_CHANNEL
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_VALUE_MAX
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_VALUE_MIN
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_READ_INTERVAL
