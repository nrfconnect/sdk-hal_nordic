/*
 * Copyright (c) 2022 - 2025, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "nrfx_gppiv1.h"
#include <helpers/nrfx_flag32_allocator.h>

#if defined(PPI_PRESENT)

#if !defined (NRFX_PPI_CHANNELS_USED) && !defined(__NRFX_DOXYGEN__)
/* Bitfield representing PPI channels used by external modules. */
#define NRFX_PPI_CHANNELS_USED 0
#endif

#if !defined(NRFX_PPI_GROUPS_USED) && !defined(__NRFX_DOXYGEN__)
/* Bitfield representing PPI groups used by external modules. */
#define NRFX_PPI_GROUPS_USED 0
#endif

#if (PPI_CH_NUM > 16) || defined(__NRFX_DOXYGEN__)
/** @brief Bitfield representing all PPI channels available to the application. */
#define NRFX_PPI_ALL_APP_CHANNELS_MASK   ((uint32_t)0xFFFFFFFFuL & ~(NRFX_PPI_CHANNELS_USED))
/** @brief Bitfield representing programmable PPI channels available to the application. */
#define NRFX_PPI_PROG_APP_CHANNELS_MASK  ((uint32_t)0x000FFFFFuL & ~(NRFX_PPI_CHANNELS_USED))
#else
#define NRFX_PPI_ALL_APP_CHANNELS_MASK   ((uint32_t)0xFFF0FFFFuL & ~(NRFX_PPI_CHANNELS_USED))
#define NRFX_PPI_PROG_APP_CHANNELS_MASK  ((uint32_t)0x0000FFFFuL & ~(NRFX_PPI_CHANNELS_USED))
#endif

/** @brief Bitfield representing all PPI groups available to the application. */
#define NRFX_PPI_ALL_APP_GROUPS_MASK     (((1uL << PPI_GROUP_NUM) - 1) & ~(NRFX_PPI_GROUPS_USED))

/** @brief Bitmask representing channels availability. */
static nrfx_atomic_t m_channels_allocated = NRFX_PPI_PROG_APP_CHANNELS_MASK;

/** @brief Bitmask representing groups availability. */
static nrfx_atomic_t m_groups_allocated = NRFX_PPI_ALL_APP_GROUPS_MASK;

bool nrfx_gppiv1_channel_check(uint8_t channel)
{
    return (nrf_ppi_channel_enable_get(NRF_PPI, (nrf_ppi_channel_t)channel) ==
            NRF_PPI_CHANNEL_ENABLED);
}

void nrfx_gppiv1_channels_disable_all(void)
{
    nrf_ppi_channels_disable_all(NRF_PPI);
}

void nrfx_gppiv1_channels_enable(uint32_t mask)
{
    nrf_ppi_channels_enable(NRF_PPI, mask);
}

void nrfx_gppiv1_channels_disable(uint32_t mask)
{
    nrf_ppi_channels_disable(NRF_PPI, mask);
}

void nrfx_gppiv1_event_endpoint_setup(uint8_t channel, uint32_t eep)
{
    nrf_ppi_event_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, eep);
}

void nrfx_gppiv1_task_endpoint_setup(uint8_t channel, uint32_t tep)
{
    nrf_ppi_task_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, tep);
}

void nrfx_gppiv1_channel_endpoints_setup(uint8_t  channel, uint32_t eep, uint32_t tep)
{
    nrf_ppi_channel_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, eep, tep);
}

void nrfx_gppiv1_channel_endpoints_clear(uint8_t channel, uint32_t eep, uint32_t tep)
{
    nrfx_gppiv1_event_endpoint_clear(channel, eep);
    nrfx_gppiv1_task_endpoint_clear(channel, tep);
}

void nrfx_gppiv1_event_endpoint_clear(uint8_t channel, uint32_t eep)
{
    (void)eep;
     nrf_ppi_event_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
}

void nrfx_gppiv1_task_endpoint_clear(uint8_t channel, uint32_t tep)
{
    (void)tep;
    nrf_ppi_task_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
}

#if defined(PPI_FEATURE_FORKS_PRESENT)
void nrfx_gppiv1_fork_endpoint_setup(uint8_t channel, uint32_t fork_tep)
{
    nrf_ppi_fork_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, fork_tep);
}

void nrfx_gppiv1_fork_endpoint_clear(uint8_t channel, uint32_t fork_tep)
{
    (void)fork_tep;
    nrf_ppi_fork_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
}
#endif

void nrfx_gppiv1_channels_group_set(uint32_t                  channel_mask,
                                  nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_channels_group_set(NRF_PPI,
                               channel_mask,
                               (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_channels_include_in_group(uint32_t                  channel_mask,
                                         nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_channels_include_in_group(NRF_PPI,
                                      channel_mask,
                                      (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_channels_remove_from_group(uint32_t                  channel_mask,
                                          nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_channels_remove_from_group(NRF_PPI,
                                       channel_mask,
                                       (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_group_clear(nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_group_clear(NRF_PPI, (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_group_enable(nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_group_enable(NRF_PPI, (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_group_disable(nrfx_gppi_channel_group_t channel_group)
{
    nrf_ppi_group_disable(NRF_PPI, (nrf_ppi_channel_group_t)channel_group);
}

void nrfx_gppiv1_task_trigger(nrfx_gppi_task_t task)
{
    nrf_ppi_task_trigger(NRF_PPI, (nrf_ppi_task_t)task);
}

uint32_t nrfx_gppiv1_task_address_get(nrfx_gppi_task_t task)
{
    return (uint32_t)nrf_ppi_task_address_get(NRF_PPI, (nrf_ppi_task_t)task);
}

nrfx_gppi_task_t nrfx_gppiv1_group_disable_task_get(nrfx_gppi_channel_group_t group)
{
    return (nrfx_gppi_task_t)nrf_ppi_group_disable_task_get(NRF_PPI, (uint8_t)group);
}

nrfx_gppi_task_t nrfx_gppiv1_group_enable_task_get(nrfx_gppi_channel_group_t group)
{
    return (nrfx_gppi_task_t)nrf_ppi_group_enable_task_get(NRF_PPI, (uint8_t)group);
}

nrfx_err_t nrfx_gppiv1_channel_alloc(uint8_t * p_channel)
{
    return nrfx_flag32_alloc(&m_channels_allocated, (uint8_t *)p_channel);
}

nrfx_err_t nrfx_gppiv1_channel_free(uint8_t channel)
{
    nrf_ppi_channel_disable(NRF_PPI, channel);

    return nrfx_flag32_free(&m_channels_allocated, channel);
}

nrfx_err_t nrfx_gppiv1_group_alloc(nrfx_gppi_channel_group_t * p_group)
{
    return nrfx_flag32_alloc(&m_groups_allocated, (uint8_t *)p_group);
}

nrfx_err_t nrfx_gppiv1_group_free(nrfx_gppi_channel_group_t group)
{
    nrf_ppi_group_disable(NRF_PPI, group);
    return nrfx_flag32_free(&m_groups_allocated, group);
}
#endif // defined(PPI_PRESENT)
