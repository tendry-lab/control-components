/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#ifdef CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#include <cstdint>
#include <cstdio>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
uint32_t HAL_GetTick(void);
#ifdef __cplusplus
}
#endif // __cplusplus

#if defined(__cplusplus) && (__cplusplus > 201703L)

#define _OCS_LOG(level, tag, fmt, ...)                                                   \
    printf("%lu %s %s: " fmt "\r\n", HAL_GetTick(), level, tag __VA_OPT__(, ) __VA_ARGS__)

#define ocs_logi(tag, fmt, ...) _OCS_LOG("I", tag, fmt __VA_OPT__(, ) __VA_ARGS__)
#define ocs_logw(tag, fmt, ...) _OCS_LOG("W", tag, fmt __VA_OPT__(, ) __VA_ARGS__)
#define ocs_loge(tag, fmt, ...) _OCS_LOG("E", tag, fmt __VA_OPT__(, ) __VA_ARGS__)
#define ocs_logd(tag, fmt, ...) _OCS_LOG("D", tag, fmt __VA_OPT__(, ) __VA_ARGS__)

#else // !(defined(__cplusplus) && (__cplusplus > 201703L))

#define _OCS_LOG(level, tag, fmt, ...)                                                   \
    printf("%lu %s %s: " fmt "\r\n", HAL_GetTick(), level, tag, ##__VA_ARGS__)

#define ocs_logi(tag, fmt, ...) _OCS_LOG("I", tag, fmt, ##__VA_ARGS__)
#define ocs_logw(tag, fmt, ...) _OCS_LOG("W", tag, fmt, ##__VA_ARGS__)
#define ocs_loge(tag, fmt, ...) _OCS_LOG("E", tag, fmt, ##__VA_ARGS__)
#define ocs_logd(tag, fmt, ...) _OCS_LOG("D", tag, fmt, ##__VA_ARGS__)

#endif // defined(__cplusplus) && (__cplusplus > 201703L)

#else // !CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#include "esp_log.h"

#define ocs_logi ESP_LOGI
#define ocs_logw ESP_LOGW
#define ocs_loge ESP_LOGE
#define ocs_logd ESP_LOGD

#endif // CONFIG_OCS_FIRMWARE_PLATFORM_STM32
