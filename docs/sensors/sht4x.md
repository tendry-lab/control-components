## Introduction

SHT4x is a digital humidity and temperature sensor. SHT4x is connected to a board with I2C interface. See the [datasheet](https://sensirion.com/media/documents/33FD6951/662A593A/HT_DS_Datasheet_SHT4x.pdf) for more details.

## Telemetry Fields

```json
{
    "sensor_sht4x_heating_count": 7,
    "sensor_sht4x_humidity": 54.87,
    "sensor_sht4x_temperature": 21.92
}
```

## HTTP API

- `bonsai-firmware.local/api/v1/sensor/sht4x/reset` - reset the sensor.
- `bonsai-firmware.local/api/v1/sensor/sht4x/heat` - activate internal heater, use with caution, the heater is designed for a maximum duty cycle of 10%, meaning the total heater-on-time should not be longer than 10% of the sensor’s lifetime.
