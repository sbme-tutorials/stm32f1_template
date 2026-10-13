# Determine the SERIES_FOLDER based on the DEVICE
if(DEVICE MATCHES "^STM32F1")
    set(SERIES_FOLDER "STM32F1xx")
else()
    message(FATAL_ERROR "Unsupported device: ${DEVICE}")
endif()

if(NOT EXISTS "${PROJECT_PATH}/STM32-base/linker/${SERIES_FOLDER}/${DEVICE}.ld")
    message(FATAL_ERROR "No linker script for device: ${DEVICE}")
endif()

# Startup files and CMSIS symbols describe density groups. For example,
# STM32F103x8 uses the STM32F103xB group but retains its own 64 KB linker script.
if(DEVICE MATCHES "^STM32F10[57]x[8BC]$")
    string(REGEX REPLACE "x[8BC]$" "xC" MAPPED_DEVICE "${DEVICE}")
elseif(DEVICE MATCHES "^STM32F100x[468B]$")
    set(MAPPED_DEVICE "STM32F100xB")
elseif(DEVICE MATCHES "^STM32F100x[CDE]$")
    set(MAPPED_DEVICE "STM32F100xE")
elseif(DEVICE MATCHES "^STM32F10[123]x[46]$")
    string(REGEX REPLACE "x[46]$" "x6" MAPPED_DEVICE "${DEVICE}")
elseif(DEVICE MATCHES "^STM32F10[123]x[8B]$")
    string(REGEX REPLACE "x[8B]$" "xB" MAPPED_DEVICE "${DEVICE}")
elseif(DEVICE MATCHES "^STM32F10[13]x[CDE]$")
    string(REGEX REPLACE "x[CDE]$" "xE" MAPPED_DEVICE "${DEVICE}")
elseif(DEVICE MATCHES "^STM32F10[13]x[FG]$")
    string(REGEX REPLACE "x[FG]$" "xG" MAPPED_DEVICE "${DEVICE}")
else ()
    message(FATAL_ERROR "No STM32F1 startup mapping for device: ${DEVICE}")
endif()

if(NOT EXISTS "${PROJECT_PATH}/STM32-base/startup/${SERIES_FOLDER}/${MAPPED_DEVICE}.s")
    message(FATAL_ERROR "No startup file for device: ${DEVICE} (mapped to ${MAPPED_DEVICE})")
endif()

string(TOLOWER "${MAPPED_DEVICE}" CMSIS_DEVICE_HEADER)
if(NOT EXISTS "${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/inc/${CMSIS_DEVICE_HEADER}.h")
    message(FATAL_ERROR "No CMSIS header for device: ${DEVICE} (mapped to ${MAPPED_DEVICE})")
endif()

message(STATUS "Device: ${DEVICE}; startup/CMSIS: ${MAPPED_DEVICE}; HAL: ${USE_HAL}")
