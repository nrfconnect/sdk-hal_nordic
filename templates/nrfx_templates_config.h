/*$$$LICENCE_NORDIC_STANDARD<2019>$$$*/

#ifndef NRFX_TEMPLATES_CONFIG_H__
#define NRFX_TEMPLATES_CONFIG_H__

#if defined(NRF54LM20A_XXAA) && defined(NRF_APPLICATION)
    #include <nrfx_config_nrf54lm20a_application.h>
#elif defined(NRF54LM20A_XXAA) && defined(NRF_FLPR)
    #include <nrfx_config_nrf54lm20a_flpr.h>
#elif defined(NRF54LM20B_XXAA) && defined(NRF_APPLICATION)
    #include <nrfx_config_nrf54lm20b_application.h>
#elif defined(NRF54LM20B_XXAA) && defined(NRF_FLPR)
    #include <nrfx_config_nrf54lm20b_flpr.h>
#else
    #include "nrfx_config_ext.h"
#endif

#endif // NRFX_TEMPLATES_CONFIG_H__
