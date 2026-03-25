# Copyright (c) 2026 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

if(DEFINED CONFIG_SOC_NRF54LM20A OR DEFINED CONFIG_SOC_NRF54LM20B)
  if(NOT CONFIG_NRF_DONT_USE_STABLE_BSP)
    message(ERROR "External BSP should be used for nRF54LM20A/B.")
  endif()
  set(NRF_USE_NRF54LM20_BSP 1)

  set(BSP_DIR ${ZEPHYR_CURRENT_MODULE_DIR})
  set(MDK_DIR ${BSP_DIR}/mdk)

  zephyr_include_directories(${BSP_DIR})
  zephyr_include_directories(${BSP_DIR}/templates)
  zephyr_include_directories(.)
endif()

zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A          NRF54LM20A_XXAA)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_CPUAPP   NRF_APPLICATION)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_CPUFLPR  NRF_FLPR)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_DEVELOP_IN_NRF54LM20B DEVELOP_IN_NRF54LM20B)
zephyr_compile_definitions_ifdef(CONFIG_SOC_COMPATIBLE_NRF54LM20A NRF54LM20A_XXAA)
zephyr_compile_definitions_ifdef(CONFIG_SOC_COMPATIBLE_NRF54LM20A_CPUAPP NRF_APPLICATION)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20B          NRF54LM20B_XXAA)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20B_CPUAPP NRF_APPLICATION)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20B_CPUFLPR NRF_FLPR)

if(DEFINED NRF_USE_NRF54LM20_BSP)
  if(CONFIG_SOC_SERIES_NRF54L OR CONFIG_SOC_SERIES_BSIM_NRF54LX)
    zephyr_library_sources(${MDK_DIR}/system_nrf54l.c)
  endif()
endif()

if(CONFIG_NRFX_GPPI AND NOT CONFIG_NRFX_GPPI_V1)
  if(CONFIG_NRF_DONT_USE_STABLE_BSP)
    if(CONFIG_SOC_COMPATIBLE_NRF54LX OR CONFIG_SOC_SERIES_NRF71)
      zephyr_library_sources(${BSP_DIR}/soc/interconnect/nrfx_gppi_d2ppi.c)
    endif()
  endif()
endif()

# Get the SVD file for the current SoC
macro(mdk_svd_ifdef feature_toggle filename)
  if(${feature_toggle})
    set(SOC_SVD_FILE ${MDK_DIR}/${filename} CACHE FILEPATH "Path to a CMSIS-SVD file")
  endif()
endmacro()

mdk_svd_ifdef(CONFIG_SOC_NRF54LM20A_CPUAPP    nrf54lm20a_application.svd)
mdk_svd_ifdef(CONFIG_SOC_NRF54LM20A_CPUFLPR   nrf54lm20a_flpr.svd)
mdk_svd_ifdef(CONFIG_SOC_NRF54LM20B_CPUAPP    nrf54lm20b_application.svd)
mdk_svd_ifdef(CONFIG_SOC_NRF54LM20B_CPUFLPR   nrf54lm20b_flpr.svd)
