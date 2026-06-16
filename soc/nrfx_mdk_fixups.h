#ifndef NRFX_MDK_FIXUPS_H__
#define NRFX_MDK_FIXUPS_H__

/**************************************************************************************************/
/* Start fixups section for NRF54LM20A_XXAA                                                       */
/**************************************************************************************************/

#if defined(NRF54LM20A_XXAA)
    #define TRNG_CLK_DIV 1
    #define I2S_MCKFREQ_FACTOR 1048576

    #define TDM_MCKCONST_FACTOR 1048576
    #define TDM_CK_DIV_FACTOR   4096
    #define TDM_MIN_TRANSFER_SIZE 3
    #define TDM_PSEL_MASK 0x8000007F
    #define TDM_TX0_CHANNEL_NEEDED

    #define KMU_TASKS_REVOKE_POLICY_Rotating 0x01UL
    #define KMU_TASKS_REVOKE_POLICY_Locked   0x02UL
    #define KMU_TASKS_REVOKE_POLICY_Revoked  0x03UL

    #define TWIM_HAS_CUSTOM_FREQUENCIES
    #define DMA_BUFFER_UNIFIED_BYTE_ACCESS 1
    #define STATIC_CPU_FREQ_CONFIG_PRESENT 1
    #define STATIC_CPU_FREQ_CONFIG_64_MHZ_PRESENT 1
    #define STATIC_CPU_FREQ_CONFIG_128_MHZ_PRESENT 1

    #if (defined(NRF_APPLICATION) && defined(NRF_TRUSTZONE_NONSECURE)) || defined(NRF_FLPR)
        #define GPIOTE20_IRQn       GPIOTE20_0_IRQn
        #define GPIOTE20_IRQHandler GPIOTE20_0_IRQHandler
        #define GPIOTE30_IRQn       GPIOTE30_0_IRQn
        #define GPIOTE30_IRQHandler GPIOTE30_0_IRQHandler
    #else
        #define GPIOTE20_IRQn       GPIOTE20_1_IRQn
        #define GPIOTE20_IRQHandler GPIOTE20_1_IRQHandler
        #define GPIOTE30_IRQn       GPIOTE30_1_IRQn
        #define GPIOTE30_IRQHandler GPIOTE30_1_IRQHandler
    #endif

    #define GPIOTE_PORT_ID 0

    #define TYPES_DOMAIN
    #define TYPES_OWNER

    #define GRTC_FORCE_EXTENDED
    #if defined(NRF_FLPR)
        #define GRTC_IRQn       GRTC_0_IRQn
        #define GRTC_IRQHandler GRTC_0_IRQHandler
    #elif defined(NRF_APPLICATION) && defined(NRF_TRUSTZONE_NONSECURE)
        #define GRTC_IRQn       GRTC_1_IRQn
        #define GRTC_IRQHandler GRTC_1_IRQHandler
    #elif defined(NRF_APPLICATION) && !defined(NRF_TRUSTZONE_NONSECURE)
        #define GRTC_IRQn       GRTC_2_IRQn
        #define GRTC_IRQHandler GRTC_2_IRQHandler
    #endif

    #if defined(NRF_FLPR)
        #define GRTC_MAIN_CC_CHANNEL 4
    #else
        #define GRTC_MAIN_CC_CHANNEL 0
    #endif

    #define SPIM_USE_H0H1_E0E1

    #define DPPI_TYPE_PPIB

    #define DELAY_RISCV_SLOWDOWN 15

    #if !defined(NRF_TRUSTZONE_NONSECURE)
        #define GPIO_DETECTMODE_ACCESSIBLE 1
    #endif
#endif

/**************************************************************************************************/
/* End fixups section for NRF54LM20A_XXAA                                                         */
/**************************************************************************************************/

/**************************************************************************************************/
/* Start fixups section for NRF54LM20B_XXAA                                                       */
/**************************************************************************************************/

#if defined(NRF54LM20B_XXAA)
    #define TRNG_CLK_DIV 1
    #define I2S_MCKFREQ_FACTOR 1048576

    #define TDM_MCKCONST_FACTOR 1048576
    #define TDM_CK_DIV_FACTOR   4096
    #define TDM_MIN_TRANSFER_SIZE 3
    #define TDM_PSEL_MASK 0x8000007F
    #define TDM_TX0_CHANNEL_NEEDED

    #define KMU_TASKS_REVOKE_POLICY_Rotating 0x01UL
    #define KMU_TASKS_REVOKE_POLICY_Locked   0x02UL
    #define KMU_TASKS_REVOKE_POLICY_Revoked  0x03UL

    #define TWIM_HAS_CUSTOM_FREQUENCIES
    #define DMA_BUFFER_UNIFIED_BYTE_ACCESS 1
    #define STATIC_CPU_FREQ_CONFIG_PRESENT 1
    #define STATIC_CPU_FREQ_CONFIG_64_MHZ_PRESENT 1
    #define STATIC_CPU_FREQ_CONFIG_128_MHZ_PRESENT 1

    #if (defined(NRF_APPLICATION) && defined(NRF_TRUSTZONE_NONSECURE)) || defined(NRF_FLPR)
        #define GPIOTE20_IRQn       GPIOTE20_0_IRQn
        #define GPIOTE20_IRQHandler GPIOTE20_0_IRQHandler
        #define GPIOTE30_IRQn       GPIOTE30_0_IRQn
        #define GPIOTE30_IRQHandler GPIOTE30_0_IRQHandler
    #else
        #define GPIOTE20_IRQn       GPIOTE20_1_IRQn
        #define GPIOTE20_IRQHandler GPIOTE20_1_IRQHandler
        #define GPIOTE30_IRQn       GPIOTE30_1_IRQn
        #define GPIOTE30_IRQHandler GPIOTE30_1_IRQHandler
    #endif

    #define GPIOTE_PORT_ID 0

    #define TYPES_DOMAIN
    #define TYPES_OWNER

    #define GRTC_FORCE_EXTENDED
    #if defined(NRF_FLPR)
        #define GRTC_IRQn       GRTC_0_IRQn
        #define GRTC_IRQHandler GRTC_0_IRQHandler
    #elif defined(NRF_APPLICATION) && defined(NRF_TRUSTZONE_NONSECURE)
        #define GRTC_IRQn       GRTC_1_IRQn
        #define GRTC_IRQHandler GRTC_1_IRQHandler
    #elif defined(NRF_APPLICATION) && !defined(NRF_TRUSTZONE_NONSECURE)
        #define GRTC_IRQn       GRTC_2_IRQn
        #define GRTC_IRQHandler GRTC_2_IRQHandler
    #endif

    #if defined(NRF_FLPR)
        #define GRTC_MAIN_CC_CHANNEL 4
    #else
        #define GRTC_MAIN_CC_CHANNEL 0
    #endif

    #define SPIM_USE_H0H1_E0E1

    #define DPPI_TYPE_PPIB

    #define DELAY_RISCV_SLOWDOWN 15

    #if !defined(NRF_TRUSTZONE_NONSECURE)
        #define GPIO_DETECTMODE_ACCESSIBLE 1
    #endif
#endif

/**************************************************************************************************/
/* End fixups section for NRF54LM20B_XXAA                                                         */
/**************************************************************************************************/

/**************************************************************************************************/
/* Start fixups section for external                                                              */
/**************************************************************************************************/

#include "soc/nrfx_mdk_fixups_ext.h"

/**************************************************************************************************/
/* End fixups section for external                                                                */
/**************************************************************************************************/

#endif // NRFX_MDK_FIXUPS_H__
