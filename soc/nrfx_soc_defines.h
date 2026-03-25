/*$$$LICENCE_NORDIC_STANDARD<2025>$$$*/

#ifndef NRFX_SOC_DEFINES_H__
#define NRFX_SOC_DEFINES_H__

#include <nrfx.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Analog pins definitions. */

#if defined(LUMOS_XXAA)
#define ANALOG_REF_INTERNAL_VAL 900
#else
#define ANALOG_REF_INTERNAL_VAL 600
#endif

/* Note: even though ifdefs are not necessary as BSP is only used for one platform,
   it is recommended to use them to facilitate adding a different SoC to the current BSP
   or moving a SoC to the STABLE BSP. */
#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define COMP_EXTERNAL_AIN_PSELS         \
        NRF_PIN_PORT_TO_PIN_NUMBER(0U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(31U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(30U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(29U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(6U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(5U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(4U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(3U, 1)
#else
    #error "Unknown device."
#endif

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define COMP_EXTERNAL_REF_PSELS         \
        NRF_PIN_PORT_TO_PIN_NUMBER(0U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(31U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(30U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(29U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(6U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(5U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(4U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(3U, 1),
#else
    #error "Unknown device."
#endif

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define LPCOMP_EXTERNAL_AIN_PSELS       \
        NRF_PIN_PORT_TO_PIN_NUMBER(0U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(31U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(30U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(29U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(6U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(5U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(4U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(3U, 1),
#else
    #error "Unknown device."
#endif

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define LPCOMP_EXTERNAL_REF_PSELS       \
        NRF_PIN_PORT_TO_PIN_NUMBER(0U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(31U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(30U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(29U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(6U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(5U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(4U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(3U, 1),
#else
    #error "Unknown device."
#endif

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define SAADC_EXTERNAL_AIN_PSELS        \
        NRF_PIN_PORT_TO_PIN_NUMBER(0U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(31U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(30U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(29U, 1), \
        NRF_PIN_PORT_TO_PIN_NUMBER(6U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(5U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(4U, 1),  \
        NRF_PIN_PORT_TO_PIN_NUMBER(3U, 1),
#else
    #error "Unknown device."
#endif

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    #define SAADC_INTERNAL_AIN_PSELS                                     \
        NRF_SAADC_INPUT_VDD,                                             \
        NRFX_SAADC_INPUT_NOT_PRESENT,                                    \
        NRF_SAADC_INPUT_AVDD,                                            \
        NRFX_COND_CODE_1(NRFX_ARG_HAS_PARENTHESIS(NRF_SAADC_INPUT_DVDD), \
            (NRF_SAADC_INPUT_DVDD,), (NRFX_SAADC_INPUT_NOT_PRESENT,))
#else
    #error "Unknown device."
#endif

/* Ram sections definitions. */

#define RAM_NON_UNIFORM_SECTION_DECLARE(i, _block, _section) {.decoded = {_block, _section}}

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
#define RAM_SECTION_UNIT_SIZE          (32UL * 1024UL)
#define RAM_UNIFORM_BLOCKS             1
#define RAM_UNIFORM_SECTIONS_PER_BLOCK 16
#define RAM_UNIFORM_SECTIONS_TOTAL     16
#endif

/* PRS boxes definitions. */

#if defined(NRF54LM20A_XXAA) || defined(NRF54LM20B_XXAA)
    // SPIM00, SPIS00, UARTE00
    #define NRFX_PRS_BOX_0_ADDR     NRF_UARTE00
    // SPIM20, SPIS20, TWIM20, TWIS20, UARTE20
    #define NRFX_PRS_BOX_1_ADDR     NRF_UARTE20
    // SPIM21, SPIS21, TWIM21, TWIS21, UARTE21
    #define NRFX_PRS_BOX_2_ADDR     NRF_UARTE21
    // SPIM22, SPIS22, TWIM22, TWIS22, UARTE22
    #define NRFX_PRS_BOX_3_ADDR     NRF_UARTE22
    // SPIM23, SPIS23, TWIM23, TWIS23, UARTE23
    #define NRFX_PRS_BOX_4_ADDR     NRF_UARTE23
    // SPIM24, SPIS24, TWIM24, TWIS24, UARTE23
    #define NRFX_PRS_BOX_5_ADDR     NRF_UARTE24
    // SPIM30, SPIS30, TWIM30, TWIS30, UARTE30
    #define NRFX_PRS_BOX_6_ADDR     NRF_UARTE30
    // COMP, LPCOMP
    #define NRFX_PRS_BOX_7_ADDR     NRF_COMP
#endif

/* Definition of macro calculating HFXO internal capacitor value. */
#define OSCILLATORS_HFXO_CAP_CALCULATE(p_ficr_reg, cap_val)                     \
      (((((p_ficr_reg->XOSC32MTRIM & FICR_XOSC32MTRIM_SLOPE_Msk)                \
         >> FICR_XOSC32MTRIM_SLOPE_Pos) + 791) * (uint32_t)(cap_val * 4 - 22) + \
        (((p_ficr_reg->XOSC32MTRIM & FICR_XOSC32MTRIM_OFFSET_Msk)               \
         >> FICR_XOSC32MTRIM_OFFSET_Pos) << 4)) >> 10)

#ifdef __cplusplus
}
#endif

#endif /* NRFX_SOC_DEFINES_H__ */
