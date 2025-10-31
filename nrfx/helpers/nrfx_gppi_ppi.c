/*
 * Copyright (c) 2025, Nordic Semiconductor ASA
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
#include <nrfx.h>

#if defined(PPI_PRESENT)
#include <hal/nrf_ppi.h>
#include <helpers/nrfx_gppi.h>

#define PPI_EP_IS_EVT(_ep) ((_ep) & NRFX_BIT(8))

static nrfx_gppi_t * p_gppi;

void nrfx_gppi_init(nrfx_gppi_t * p_instance)
{
    p_gppi = p_instance;
}

static int alloc_ch(nrfx_atomic_t *p_mask, uint8_t *p_channel)
{
    int rv;

    NRFX_CRITICAL_SECTION_ENTER();
    if (*p_mask == 0)
    {
        rv = -ENOMEM;
    }
    else if (p_channel)
    {
        if (*p_mask & NRFX_BIT(*p_channel)) {
            rv = (int)*p_channel;
            *p_mask &= ~NRFX_BIT(rv);
        } else {
            rv = -ENOMEM;
        }
    }
    else
    {
        rv = (int)(31 - NRFX_CLZ(*p_mask));
        *p_mask &= ~NRFX_BIT(rv);
    }
    NRFX_CRITICAL_SECTION_EXIT();
    return rv;
}

static void free_ch(nrfx_atomic_t *p_mask, uint32_t ch)
{
    uint32_t prev = NRFX_ATOMIC_FETCH_OR(p_mask, NRFX_BIT(ch));
    (void)prev;
    NRFX_ASSERT((prev & NRFX_BIT(ch)) == 0);
}

int nrfx_gppi_domain_conn_alloc(uint32_t producer, uint32_t consumer,
                                nrfx_gppi_handle_t * p_handle)
{
    NRFX_ASSERT(p_gppi != NULL);
    (void)producer;
    (void)consumer;
    int ch = alloc_ch(&p_gppi->ch_mask, NULL);

    if (ch < 0) {
        return ch;
    }

    *p_handle = (nrfx_gppi_handle_t)ch;
    return 0;
}

int nrfx_gppi_conn_alloc(uint32_t eep, uint32_t tep, nrfx_gppi_handle_t * p_handle)
{
    int err = nrfx_gppi_domain_conn_alloc(0, 0, p_handle);

    if (err < 0)
    {
        return err;
    }
    nrf_ppi_channel_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)*p_handle, eep, tep);

    return 0;
}

int nrfx_gppi_channel_alloc(uint32_t node_id, uint8_t *p_channel)
{
    NRFX_ASSERT(p_gppi != NULL);
    (void)node_id;
    return alloc_ch(&p_gppi->ch_mask, p_channel);
}

void nrfx_gppi_channel_free(uint32_t node_id, uint8_t channel)
{
    (void)node_id;

    free_ch(&p_gppi->ch_mask, (uint32_t)channel);
}

void nrfx_gppi_conn_free(uint32_t eep, uint32_t tep, nrfx_gppi_handle_t handle)
{
    (void)eep;
    (void)tep;
    NRFX_ASSERT(p_gppi != NULL);

#ifdef PPI_FEATURE_FORKS_PRESENT
    nrf_ppi_fork_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)handle, 0);
#endif
    nrf_ppi_task_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)handle, 0);
    nrf_ppi_event_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)handle, 0);
    free_ch(&p_gppi->ch_mask, handle);
}

void nrfx_gppi_domain_conn_free(nrfx_gppi_handle_t handle)
{
    free_ch(&p_gppi->ch_mask, handle);
}

int nrfx_gppi_group_alloc(uint32_t * p_ep, size_t ep_cnt, nrfx_gppi_group_handle_t * p_handle)
{
    NRFX_ASSERT(p_gppi != NULL);
    int err = alloc_ch(&p_gppi->group_mask, NULL);
    if (err < 0) {
        return err;
    }

    *p_handle = (nrfx_gppi_group_handle_t)err;
    uint32_t mask = 0;

    for (size_t i = 0; i < ep_cnt; i++)
    {
        mask |= NRFX_BIT(nrfx_gppi_ep_channel_get(p_ep[i]));
    }

    nrf_ppi_channels_include_in_group(NRF_PPI, mask, (nrf_ppi_channel_group_t)*p_handle);

    return 0;
}

int nrfx_gppi_group_channel_alloc(uint32_t node_id, uint8_t *p_channel)
{
    NRFX_ASSERT(p_gppi != NULL);
    (void)node_id;
    return alloc_ch(&p_gppi->group_mask, p_channel);
}


void nrfx_gppi_group_free(nrfx_gppi_group_handle_t handle)
{
    nrf_ppi_group_clear(NRF_PPI, (nrf_ppi_channel_group_t)handle);
    free_ch(&p_gppi->group_mask, handle);
}

void nrfx_gppi_group_channel_free(uint32_t node_id, uint8_t channel)
{
    (void)node_id;
    free_ch(&p_gppi->group_mask, (uint32_t)channel);
}

int nrfx_gppi_ep_to_ch_attach(uint32_t ep, uint8_t channel)
{
    nrf_ppi_channel_t ch = (nrf_ppi_channel_t)channel;

    if (PPI_EP_IS_EVT(ep)) {
        if (NRF_PPI->CH[ch].EEP != 0) {
            return -ENOTSUP;
        }
        NRF_PPI->CH[ch].EEP = ep;
        return 0;
    }

    if (nrf_ppi_task_endpoint_get(NRF_PPI, ch) == 0) {
        nrf_ppi_task_endpoint_setup(NRF_PPI, ch, ep);
        return 0;
    }

#ifdef PPI_FEATURE_FORKS_PRESENT
    if (nrf_ppi_fork_endpoint_get(NRF_PPI, ch) != 0) {
        return -EBUSY;
    }
    nrf_ppi_fork_endpoint_setup(NRF_PPI, ch, ep);
    return 0;
#else
    return -ENOTSUP;
#endif
}

int nrfx_gppi_ep_attach(uint32_t ep, nrfx_gppi_handle_t handle)
{
    return nrfx_gppi_ep_to_ch_attach(ep, (uint8_t)handle);
}

int nrfx_gppi_ep_channel_get(uint32_t ep)
{
    if (!PPI_EP_IS_EVT(ep))
    {
#ifdef PPI_FEATURE_FORKS_PRESENT
        for (int i = 0; i < PPI_CH_NUM; i++)
        {
            if (nrf_ppi_fork_endpoint_get(NRF_PPI, (nrf_ppi_channel_t)i) == ep)
            {
                return i;
            }
        }
#endif
        for (int i = 0; i < PPI_CH_NUM; i++)
        {
            if (NRF_PPI->CH[i].TEP == ep)
            {
                return (int)i;
            }
        }
    }
    else
    {
        for (int i = 0; i < PPI_CH_NUM; i++)
        {
            if (NRF_PPI->CH[i].EEP == ep)
            {
                return i;
            }
        }
    }

    return -EINVAL;
}

void nrfx_gppi_ep_ch_clear(uint32_t ep, uint8_t channel)
{
    if (PPI_EP_IS_EVT(ep)) {
        nrf_ppi_event_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
        return;
    }
#ifdef PPI_FEATURE_FORKS_PRESENT
    if (nrf_ppi_fork_endpoint_get(NRF_PPI, (nrf_ppi_channel_t)channel) == ep) {
        nrf_ppi_fork_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
        return;
    }
#endif
    nrf_ppi_task_endpoint_setup(NRF_PPI, (nrf_ppi_channel_t)channel, 0);
}

void nrfx_gppi_ep_clear(uint32_t ep)
{
    int ch = nrfx_gppi_ep_channel_get(ep);

    if (ch < 0) {
        return;
    }
    nrfx_gppi_ep_ch_clear(ep, (uint8_t)ch);
}

void nrfx_gppi_ep_enable(uint32_t ep)
{
    (void)nrfx_gppi_ep_chan_enable(ep);
}

void nrfx_gppi_ep_disable(uint32_t ep)
{
    (void)nrfx_gppi_ep_chan_disable(ep);
}

void nrfx_gppi_conn_enable(nrfx_gppi_handle_t handle)
{
    nrf_ppi_channel_enable(NRF_PPI, (nrf_ppi_channel_t)handle);
}

void nrfx_gppi_conn_disable(nrfx_gppi_handle_t handle)
{
    nrf_ppi_channel_disable(NRF_PPI, (nrf_ppi_channel_t)handle);
}

bool nrfx_gppi_chan_is_enabled(uint32_t domain_id, uint32_t ch)
{
    (void)domain_id;
    return nrf_ppi_channel_enable_get(NRF_PPI, (nrf_ppi_channel_t)ch) == NRF_PPI_CHANNEL_ENABLED;
}

void nrfx_gppi_channels_enable(uint32_t domain_id, uint32_t ch_mask)
{
    (void)domain_id;
    nrf_ppi_channels_enable(NRF_PPI, ch_mask);
}

void nrfx_gppi_channels_disable(uint32_t domain_id, uint32_t ch_mask)
{
    (void)domain_id;
    nrf_ppi_channels_disable(NRF_PPI, ch_mask);
}

int nrfx_gppi_domain_channel_get(nrfx_gppi_handle_t handle, uint32_t domain_id)
{
    (void)domain_id;
    return (int)handle;
}

void nrfx_gppi_group_ch_add(nrfx_gppi_group_handle_t handle, uint32_t channel)
{
    nrf_ppi_channel_include_in_group(NRF_PPI, (nrf_ppi_channel_t)channel,
                     (nrf_ppi_channel_group_t)handle);
}

void nrfx_gppi_group_ch_remove(nrfx_gppi_group_handle_t handle, uint32_t channel)
{
    nrf_ppi_channel_remove_from_group(NRF_PPI, (nrf_ppi_channel_t)channel,
                      (nrf_ppi_channel_group_t)handle);
}

uint32_t nrfx_gppi_group_channels_get(nrfx_gppi_group_handle_t handle)
{
    uint32_t group = (uint32_t)handle;

    return NRF_PPI->CHG[group];
}

void nrfx_gppi_group_enable(nrfx_gppi_group_handle_t handle)
{
    nrf_ppi_group_enable(NRF_PPI, (nrf_ppi_channel_group_t)handle);
}

void nrfx_gppi_group_disable(nrfx_gppi_group_handle_t handle)
{
    nrf_ppi_group_disable(NRF_PPI, (nrf_ppi_channel_group_t)handle);
}

uint32_t nrfx_gppi_group_task_en_addr(nrfx_gppi_group_handle_t handle)
{
    return nrf_ppi_task_group_enable_address_get(NRF_PPI, (nrf_ppi_channel_group_t)handle);
}

uint32_t nrfx_gppi_group_task_dis_addr(nrfx_gppi_group_handle_t handle)
{
    return nrf_ppi_task_group_disable_address_get(NRF_PPI, (nrf_ppi_channel_group_t)handle);
}
#endif
