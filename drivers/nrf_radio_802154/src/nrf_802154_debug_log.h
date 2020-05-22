/*
 * Copyright (c) 2019 - 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRF_802154_DEBUG_LOG_H_
#define NRF_802154_DEBUG_LOG_H_

#include <stdint.h>

#include "nrf_802154_config.h"
#include "nrf_802154_debug_log_codes.h"

/**@def NRF_802154_DEBUG_LOG_BUFFER_LEN
 * @brief Capacity of debug log buffer
 * @note This value must be power of 2
 */
#ifndef NRF_802154_DEBUG_LOG_BUFFER_LEN
#define NRF_802154_DEBUG_LOG_BUFFER_LEN 1024U
#endif

/**@def NRF_802154_DEBUG_LOG_BLOCKS_IRQS
 * @brief Configures if writing to log buffer is performed with interrupts disabled.
 * Setting this macro to 1 has following consequences:
 * - Interrupts are automatically disabled during write to log buffer. This ensures
 *   thread-safety and always coherent log
 * - Higher priority interrupts may be delayed, so logging has impact on timing
 * Setting this macto to 0 has following consequences:
 * - Interrupts are NOT disabled during write to log buffer. This may lead to missing
 *   logs if higher priority interrupt preempts current write log operation.
 * - Logging does not introduce delay to execution of higher priority interrupts.
 */
#ifndef NRF_802154_DEBUG_LOG_BLOCKS_INTERRUPTS
#define NRF_802154_DEBUG_LOG_BLOCKS_INTERRUPTS 0
#endif

/**@def NRF_802154_LOG_VERBOSITY
 * @brief Current verbosity level of generated logs.
 * Define this macro in your C file before inclusion of nrf_802154_debug_log.h to
 * set verbosity level per-module basis.
 */
#ifndef NRF_802154_LOG_VERBOSITY
#define NRF_802154_LOG_VERBOSITY      NRF_802154_LOG_VERBOSITY_LOW
#endif

#define NRF_802154_LOG_VERBOSITY_NONE 0
#define NRF_802154_LOG_VERBOSITY_LOW  1
#define NRF_802154_LOG_VERBOSITY_HIGH 2

#if (NRF_802154_DEBUG_LOG_BLOCKS_INTERRUPTS)

#include "nrf_802154_utils.h"

#define nrf_802154_debug_log_saved_interrupt_state_variable(var_name) \
    nrf_802154_mcu_critical_state_t var_name

#define nrf_802154_debug_log_disable_interrupts(var_name) \
    nrf_802154_mcu_critical_enter(var_name)               \

#define nrf_802154_debug_log_restore_interrupts(var_name) \
    nrf_802154_mcu_critical_exit(var_name)                \

#else

#define nrf_802154_debug_log_saved_interrupt_state_variable(var_name) /* empty macro */
#define nrf_802154_debug_log_disable_interrupts(var_name) \
    do                                                    \
    {                                                     \
    }                                                     \
    while (0)
#define nrf_802154_debug_log_restore_interrupts(var_name) \
    do                                                    \
    {                                                     \
    }                                                     \
    while (0)

#endif

/**@brief Checks if provided @c verbosity has value allowing the module to record a log. */
#define nrf_802154_debug_log_verbosity_allows(verbosity) \
    (((verbosity) > 0) && ((verbosity) <= NRF_802154_LOG_VERBOSITY))

#if !defined(CU_TEST) && (ENABLE_DEBUG_LOG)

extern volatile uint32_t nrf_802154_debug_log_buffer[NRF_802154_DEBUG_LOG_BUFFER_LEN];
extern volatile uint32_t nrf_802154_debug_log_ptr;

/**@brief Writes one word into debug log buffer
 */
#define nrf_802154_debug_log_write_raw(value)                                                   \
    do                                                                                          \
    {                                                                                           \
        uint32_t nrf_802154_debug_log_write_raw_value = (value);                                \
                                                                                                \
        nrf_802154_debug_log_saved_interrupt_state_variable(nrf_802154_debug_log_write_raw_sv); \
        nrf_802154_debug_log_disable_interrupts(nrf_802154_debug_log_write_raw_sv);             \
                                                                                                \
        uint32_t nrf_802154_debug_log_write_raw_ptr = nrf_802154_debug_log_ptr;                 \
                                                                                                \
        nrf_802154_debug_log_buffer[nrf_802154_debug_log_write_raw_ptr] =                       \
            nrf_802154_debug_log_write_raw_value;                                               \
        nrf_802154_debug_log_write_raw_ptr =                                                    \
            (nrf_802154_debug_log_write_raw_ptr + 1U) & (NRF_802154_DEBUG_LOG_BUFFER_LEN - 1U); \
        nrf_802154_debug_log_ptr = nrf_802154_debug_log_write_raw_ptr;                          \
                                                                                                \
        nrf_802154_debug_log_restore_interrupts(nrf_802154_debug_log_write_raw_sv);             \
    }                                                                                           \
    while (0)

#else // !defined(CU_TEST) && (ENABLE_DEBUG_LOG)

#define nrf_802154_debug_log_write_raw(value) \
    do                                        \
    {                                         \
    }                                         \
    while (0)

#endif // !defined(CU_TEST) && (ENABLE_DEBUG_LOG)

/**@brief Bit shift of field "log type" in log word. */
#define NRF_802154_DEBUG_LOG_TYPE_BITPOS      28

/**@brief Bit shift of field "module id" in log word. */
#define NRF_802154_DEBUG_LOG_MODULE_ID_BITPOS 22

/**@brief Bit shift of field "event id" in log word. */
#define NRF_802154_DEBUG_LOG_EVENT_ID_BITPOS  16

/**@brief Records log about entry to a function.
 * @param verbosity     Verbosity level of the module in which log is recorded required to emit log.
 */
#define nrf_802154_log_function_enter(verbosity)                                             \
    do                                                                                       \
    {                                                                                        \
        if (nrf_802154_debug_log_verbosity_allows(verbosity))                                \
        {                                                                                    \
            nrf_802154_debug_log_write_raw(                                                  \
                ((NRF_802154_LOG_TYPE_FUNCTION_ENTER) << NRF_802154_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_DEBUG_LOG_MODULE_ID_BITPOS) |          \
                ((uint32_t)((uintptr_t)(__func__)) << 0));                                   \
        }                                                                                    \
    }                                                                                        \
    while (0)

/**@brief Records log about exit from a function.
 * @param verbosity     Verbosity level of the module in which log is recorded required to emit log.
 */
#define nrf_802154_log_function_exit(verbosity)                                             \
    do                                                                                      \
    {                                                                                       \
        if (nrf_802154_debug_log_verbosity_allows(verbosity))                               \
        {                                                                                   \
            nrf_802154_debug_log_write_raw(                                                 \
                ((NRF_802154_LOG_TYPE_FUNCTION_EXIT) << NRF_802154_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_DEBUG_LOG_MODULE_ID_BITPOS) |         \
                ((uint32_t)((uintptr_t)(__func__)) << 0));                                  \
        }                                                                                   \
    }                                                                                       \
    while (0)

/**@brief Records log about event (with parameter) related to current module.
 * @param verbosity         Verbosity level of the module in which log is recorded required to emit log.
 * @param local_event_id    Event identifier whose meaning is defined within scope of the module
 *                          in which log is recorded. Possible values 0...63
 * @param param_u16         Additional parameter to be logged with event. Meaning
 *                          of the parameter is defined by the module in which
 *                          the log is recorded and event_id.
 */
#define nrf_802154_log_local_event(verbosity, local_event_id, param_u16)                  \
    do                                                                                    \
    {                                                                                     \
        if (nrf_802154_debug_log_verbosity_allows(verbosity))                             \
        {                                                                                 \
            nrf_802154_debug_log_write_raw(                                               \
                ((NRF_802154_LOG_TYPE_LOCAL_EVENT) << NRF_802154_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_DEBUG_LOG_MODULE_ID_BITPOS) |       \
                ((local_event_id) << NRF_802154_DEBUG_LOG_EVENT_ID_BITPOS) |              \
                ((uint16_t)(param_u16) << 0));                                            \
        }                                                                                 \
    }                                                                                     \
    while (0)

/**@brief Records log about event (with parameter) related to global resource.
 * @param verbosity     Verbosity level of the module in which log is recorded required to emit log.
 * @param event_id      Event identifier whose meaning is defined globally. Possible values 0...63
 * @param param_u16     Additional parameter to be logged with event. Meaning
 *                      of the parameter is defined by value of global_event_id.
 */
#define nrf_802154_log_global_event(verbosity, global_event_id, param_u16)                 \
    do                                                                                     \
    {                                                                                      \
        if (nrf_802154_debug_log_verbosity_allows(verbosity))                              \
        {                                                                                  \
            nrf_802154_debug_log_write_raw(                                                \
                ((NRF_802154_LOG_TYPE_GLOBAL_EVENT) << NRF_802154_DEBUG_LOG_TYPE_BITPOS) | \
                ((NRF_802154_MODULE_ID) << NRF_802154_DEBUG_LOG_MODULE_ID_BITPOS) |        \
                ((global_event_id) << NRF_802154_DEBUG_LOG_EVENT_ID_BITPOS) |              \
                ((uint16_t)(param_u16) << 0));                                             \
        }                                                                                  \
    }                                                                                      \
    while (0)

#endif /* NRF_802154_DEBUG_LOG_H_ */
