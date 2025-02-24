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

http "bonsai-firmware.local/api/v1/config/sensor/analog"

```json
[
    {
        "id": "ldr_a0",
        "max": 950,
        "min": 50
    },
    {
        "id": "soil_a0",
        "max": 900,
        "min": 100
    }
]
```

**Get LDR sensor configuration**

http "bonsai-firmware.local/api/v1/config/sensor/analog?id=ldr_a0"

```json
{
    "min": 950,
    "max": 50
}
```

**Set LDR sensor configuration**

http "bonsai-firmware.local/api/v1/config/sensor/analog?id=ldr_a0&min=200&max=800"

```txt
OK
```

- id - unique sensor identifier.
- max - maximum possible sensor value.
- min - minimum possible sensor value.

**Reset LDR sensor configuration**

http "bonsai-firmware.local/api/v1/config/sensor/analog/id=ldr_a0&reset=1"

```txt
OK
```

- id - unique sensor identifier.

## Firmware Configuration Options

- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_ADC_CHANNEL
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_VALUE_MAX
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_VALUE_MIN
- CONFIG_BONSAI_FIRMWARE_SENSOR_LDR_ANALOG_READ_INTERVAL
