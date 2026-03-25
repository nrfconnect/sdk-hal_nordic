/*$$$LICENCE_NORDIC_STANDARD<2023>$$$*/

#ifndef NRFX_CONFIG_EXT_H__
#define NRFX_CONFIG_EXT_H__

#if defined(NRF54LM20_XXAA)
    /* Note: this is a place for cores, that are already known to be internal, if there are any. */
#else
    #error "Unknown device."
#endif

#endif // NRFX_CONFIG_EXT_H__
