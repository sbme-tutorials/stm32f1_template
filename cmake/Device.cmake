set(DEVICE                  STM32F103x8 CACHE STRING "Select your device")
set(EXTERNAL_CLOCK          8000000 CACHE STRING "HSE frequency in Hz")
option(USE_HAL "Enable HAL" OFF) # ON|OFF

set(SERIES_CPU        cortex-m3)
include(cmake/MappedTarget.cmake)

set(CPU_PARAMETERS ${CPU_PARAMETERS}
        -mthumb
        -mcpu=${SERIES_CPU}
        -mfloat-abi=soft
        "$<$<CONFIG:Debug>:-O0>"
        "$<$<CONFIG:Release>:-Os>"
        "$<$<CONFIG:RelWithDebInfo>:-Og>"
        "$<$<CONFIG:MinSizeRel>:-Os>"
        -Wall
        -Wextra
        -g
        -gdwarf-2
)

set(COMPILER_DEFINE ${COMPILER_DEFINE}
        ${MAPPED_DEVICE}
        "$<$<CONFIG:Release>:NDEBUG>"
        HSE_VALUE=${EXTERNAL_CLOCK}
        CALL_ARM_SYSTEM_INIT
)

if(USE_HAL)
    set(COMPILER_DEFINE  ${COMPILER_DEFINE} USE_HAL_DRIVER)
endif ()
