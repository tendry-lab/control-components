## Introduction

Soil status monitoring includes:
- Current soil status: Error/Dry/Depletion/Wet/Saturated
- Current soil status duration, in seconds
- Previous soil status: Error/Dry/Depletion/Wet/Saturated
- Previous soil status duration, in seconds
- Current soil status status progress, 0 - beginning of the status, 100 - end of the status
- Soil status statistics are written to flash once per configured interval (30min - 2h)

```json
{
    "curr_status": "Wet",
    "curr_status_dur": 300,
    "moisture": 64,
    "prev_status": "Saturated",
    "prev_status_dur": 300,
    "raw": 347,
    "status_progress": 75,
    "voltage": 1267,
    "write_count": 1146
}
```
