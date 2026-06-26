/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#ifndef pdTICKS_TO_MS
#define pdTICKS_TO_MS(xTicks)                                                            \
    ((TickType_t)((uint64_t)(xTicks) * 1000 / configTICK_RATE_HZ))
#endif // !pdTICKS_TO_MS

#include "FreeRTOS.h"
#include "event_groups.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#else // !CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#endif // CONFIG_OCS_FIRMWARE_PLATFORM_STM32
