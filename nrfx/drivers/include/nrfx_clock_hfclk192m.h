/*$$$LICENCE_NORDIC_STANDARD<2016>$$$*/

#ifndef NRFX_CLOCK_HFCLK192M_H__
#define NRFX_CLOCK_HFCLK192M_H__

#include <nrfx.h>
#include <hal/nrf_clock.h>

#if NRF_CLOCK_HAS_HFCLK192M

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrfx_clock_hfclk192m HFCLK192M driver
 * @{
 * @ingroup nrf_clock_hfclk192m
 * @brief   HFCLK192M peripheral driver.
 */

/** @brief Clock event. */
#define NRFX_CLOCK_HFCLK192M_EVT_HFCLK192M_STARTED \
  NRFX_BITMASK_TO_BITPOS(NRF_CLOCK_INT_HF192M_STARTED_MASK)

/** @brief Clock event handler. */
typedef void (*nrfx_clock_hfclk192m_event_handler_t)(void);

/**
 * @brief Function for initializing internal structures in the nrfx_clock module.
 *
 * After initialization, the module is in power off state (clocks are not started).
 *
 * @param[in] event_handler Event handler provided by the user.
 *                          If not provided, driver works in blocking mode.
 *
 * @retval NRFX_SUCCESS       The procedure is successful.
 * @retval NRFX_ERROR_ALREADY The driver is already initialized.
 */
nrfx_err_t nrfx_clock_hfclk192m_init(nrfx_clock_hfclk192m_event_handler_t event_handler);

/** @brief Function for uninitializing the clock module. */
void nrfx_clock_hfclk192m_uninit(void);

/**
 * @brief Function for checking if the clock driver is initialized.
 *
 * @retval true  Driver is already initialized.
 * @retval false Driver is not initialized.
 */
bool nrfx_clock_hfclk192m_init_check(void);

/** @brief Function for starting the specified clock domain. */
void nrfx_clock_hfclk192m_start(void);

/** @brief Function for stopping the specified clock domain. */
void nrfx_clock_hfclk192m_stop(void);

/**
 * @brief Function for checking the specified clock domain state.
 *
 * XTAL source is assumed for domains with multiple sources.
 *
 * @param[out] p_clk_src Pointer to a clock source that is running.
 *
 * @retval true  The clock domain is running.
 * @retval false The clock domain is not running.
 */
NRFX_STATIC_INLINE bool nrfx_clock_hfclk192m_running_check(nrf_clock_hfclk_t * p_clk_src);

#if defined(CLOCK_FEATURE_HFCLK_DIVIDE_PRESENT) || NRF_CLOCK_HAS_HFCLK192M || \
    defined(__NRFX_DOXYGEN__)
/**
 * @brief Function for setting the specified clock domain divider.
 *
 * @param[in] div New divider for the clock domain.
 *
 * @retval NRFX_SUCCESS             Divider successfully set.
 * @retval NRFX_ERROR_NOT_SUPPORTED Domain does not support setting the divider.
 * @retval NRFX_ERROR_INVALID_PARAM Divider not supported by the specified domain.
 */
nrfx_err_t nrfx_clock_hfclk192m_divider_set(nrf_clock_hfclk_div_t div);

/**
 * @brief Function for getting the specified clock domain divider.
 *
 * @return Current divider for the specified clock domain.
 */

NRFX_STATIC_INLINE nrf_clock_hfclk_div_t nrfx_clock_hfclk192m_divider_get(void);
#endif

#ifndef NRFX_DECLARE_ONLY

#if defined(CLOCK_FEATURE_HFCLK_DIVIDE_PRESENT) || NRF_CLOCK_HAS_HFCLK192M
NRFX_STATIC_INLINE nrf_clock_hfclk_div_t nrfx_clock_hfclk192m_divider_get(void)
{
    return nrf_clock_hfclk192m_div_get(NRF_CLOCK);
}
#endif // defined(CLOCK_FEATURE_HFCLK_DIVIDE_PRESENT) || NRF_CLOCK_HAS_HFCLK192M

NRFX_STATIC_INLINE bool nrfx_clock_hfclk192m_running_check(nrf_clock_hfclk_t * p_clk_src)
{
    return nrf_clock_is_running(NRF_CLOCK, NRF_CLOCK_DOMAIN_HFCLK192M, (void*)p_clk_src);
}

#endif // NRFX_DECLARE_ONLY

/** @} */

void nrfx_clock_hfclk192m_irq_handler(void);

#ifdef __cplusplus
}
#endif

#endif // NRF_CLOCK_HAS_HFCLK192M

#endif // NRFX_CLOCK_HFCLK192M_H__
