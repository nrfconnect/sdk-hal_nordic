/*$$$LICENCE_NORDIC_STANDARD<2017>$$$*/

#ifndef NRFX_IRQS_H__
#define NRFX_IRQS_H__

#if defined(NRF54LM20A_XXAA) && defined(NRF_APPLICATION)
    #include "soc/irqs/nrfx_irqs_nrf54lm20a_application.h"
#elif defined(NRF54LM20A_XXAA) && defined(NRF_FLPR)
    #include "soc/irqs/nrfx_irqs_nrf54lm20a_flpr.h"
#elif defined(NRF54LM20B_XXAA) && defined(NRF_APPLICATION)
    #include "soc/irqs/nrfx_irqs_nrf54lm20b_application.h"
#elif defined(NRF54LM20B_XXAA) && defined(NRF_FLPR)
    #include "soc/irqs/nrfx_irqs_nrf54lm20b_flpr.h"
#else
    #include "soc/irqs/nrfx_irqs_ext.h"
#endif

#endif // NRFX_IRQS_H__
