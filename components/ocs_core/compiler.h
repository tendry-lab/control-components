/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#ifndef likely
#define likely(x) __builtin_expect(!!(x), 1)
#endif // likely

#ifndef unlikely
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif // unlikely

#else // !CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#include "esp_compiler.h"

#endif // CONFIG_OCS_FIRMWARE_PLATFORM_STM32
