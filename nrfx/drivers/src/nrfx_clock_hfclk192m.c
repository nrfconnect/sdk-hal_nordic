/*$$$LICENCE_NORDIC_STANDARD<2016>$$$*/

#include <nrfx.h>

#include <nrfx_clock_hfclk192m.h>

#if NRF_CLOCK_HAS_HFCLK192M

#define NRFX_LOG_MODULE CLOCK_HFCLK192M
#include <nrfx_log.h>

/** @brief CLOCK control block. */
typedef struct
{
    nrfx_clock_hfclk192m_event_handler_t event_handler;
    bool                                 module_initialized; /*< Indicate the state of module */
} nrfx_clock_hfclk192m_cb_t;

static nrfx_clock_hfclk192m_cb_t m_clock_cb;

static void clock_stop(void)
{
    nrf_clock_int_disable(NRF_CLOCK, NRF_CLOCK_INT_HF192M_STARTED_MASK);
    nrf_clock_task_trigger(NRF_CLOCK, NRF_CLOCK_TASK_HFCLK192MSTOP);
    nrf_clock_event_clear(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED);

    bool stopped;
    NRFX_WAIT_FOR((!nrfx_clock_hfclk192m_running_check(NULL)), 10000, 1, stopped);
    if (!stopped)
    {
        NRFX_LOG_ERROR("Failed to stop clock domain: NRF_CLOCK_DOMAIN_HFCLK192M.");
    }
}

nrfx_err_t nrfx_clock_hfclk192m_init(nrfx_clock_hfclk192m_event_handler_t event_handler)
{
    nrfx_err_t err_code = NRFX_SUCCESS;
    if (m_clock_cb.module_initialized)
    {
        err_code = NRFX_ERROR_ALREADY;
        NRFX_LOG_INFO("Function: %s, error code: %s.",
                      __func__,
                      NRFX_LOG_ERROR_STRING_GET(err_code));
        return err_code;
    }
    else
    {
        m_clock_cb.event_handler = event_handler;
        m_clock_cb.module_initialized = true;
    }

    nrf_clock_hfclk192m_src_set(NRF_CLOCK, (nrf_clock_hfclk_t)NRFX_CLOCK_CONFIG_HFCLK192M_SRC);

    return err_code;
}

void nrfx_clock_hfclk192m_uninit(void)
{
    NRFX_ASSERT(m_clock_cb.module_initialized);

    clock_stop();

    m_clock_cb.module_initialized = false;
    NRFX_LOG_INFO("Uninitialized.");
}

bool nrfx_clock_hfclk192m_init_check(void)
{
    return m_clock_cb.module_initialized;
}

void nrfx_clock_hfclk192m_start(void)
{
    NRFX_ASSERT(m_clock_cb.module_initialized);

    nrf_clock_event_clear(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED);
    nrf_clock_task_trigger(NRF_CLOCK, NRF_CLOCK_TASK_HFCLK192MSTART);
    if (m_clock_cb.event_handler)
    {
        nrf_clock_int_enable(NRF_CLOCK, NRF_CLOCK_INT_HF192M_STARTED_MASK);
    }
    else
    {
        while (!nrf_clock_event_check(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED))
        {}
        nrf_clock_event_clear(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED);
    }
}

void nrfx_clock_hfclk192m_stop(void)
{
    NRFX_ASSERT(m_clock_cb.module_initialized);

    clock_stop();
}

#if defined(CLOCK_FEATURE_HFCLK_DIVIDE_PRESENT) || NRF_CLOCK_HAS_HFCLK192M
nrfx_err_t nrfx_clock_hfclk192m_divider_set(nrf_clock_hfclk_div_t div)
{
    if (div > NRF_CLOCK_HFCLK_DIV_4)
    {
        return NRFX_ERROR_INVALID_PARAM;
    }
    else
    {
        nrf_clock_hfclk192m_div_set(NRF_CLOCK, div);
    }
    return NRFX_SUCCESS;
}
#endif

void nrfx_clock_hfclk192m_irq_handler(void)
{
    if (!nrf_clock_int_enable_check(NRF_CLOCK, NRF_CLOCK_INT_HF192M_STARTED_MASK) ||
        !nrf_clock_event_check(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED))
    {
        return;
    }

    nrf_clock_event_clear(NRF_CLOCK, NRF_CLOCK_EVENT_HFCLK192MSTARTED);
    nrf_clock_int_disable(NRF_CLOCK, NRF_CLOCK_INT_HF192M_STARTED_MASK);
    NRFX_LOG_DEBUG("Event: HF192M_STARTED");

    m_clock_cb.event_handler();
}

#endif // NRF_CLOCK_HAS_HFCLK192M
