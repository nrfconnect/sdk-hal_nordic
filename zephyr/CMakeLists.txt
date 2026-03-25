# Copyright (c) 2026 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

set(MDK_DIR ${ZEPHYR_CURRENT_MODULE_DIR}/mdk)

zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A          NRF54LM20A_XXAA)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_CPUAPP   NRF_APPLICATION)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_CPUFLPR  NRF_FLPR)
zephyr_compile_definitions_ifdef(CONFIG_SOC_NRF54LM20A_DEVELOP_IN_NRF54LM20B DEVELOP_IN_NRF54LM20B)
zephyr_compile_definitions_ifdef(CONFIG_SOC_COMPATIBLE_NRF54LM20A NRF54LM20A_XXAA)
zephyr_compile_definitions_ifdef(CONFIG_SOC_COMPATIBLE_NRF54LM20A_CPUAPP NRF_APPLICATION)

if(CONFIG_SOC_SERIES_NRF54L OR CONFIG_SOC_SERIES_BSIM_NRF54LX)
  zephyr_library_sources(${MDK_DIR}/system_nrf54l.c)
endif()

# Get the SVD file for the current SoC
macro(mdk_svd_ifdef feature_toggle filename)
  if(${feature_toggle})
    set(SOC_SVD_FILE ${MDK_DIR}/${filename} CACHE FILEPATH "Path to a CMSIS-SVD file")
  endif()
endmacro()

mdk_svd_ifdef(CONFIG_SOC_NRF54LM20B_CPUAPP    nrf54lm20b_application.svd)
mdk_svd_ifdef(CONFIG_SOC_NRF54LM20B_CPUFLPR   nrf54lm20b_flpr.svd)
