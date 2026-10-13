# Scan application, driver, board and utility sources. This avoids scanning
# vendor code or compiler-generated C files in build directories.
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
        "${PROJECT_PATH}/src/*.c"
        "${PROJECT_PATH}/src/*.s"
        "${PROJECT_PATH}/src/*.S"
        "${PROJECT_PATH}/Drivers/*.c"
        "${PROJECT_PATH}/BSP/*.c"
        "${PROJECT_PATH}/Lib/*.c"
)

if(USE_HAL)
    # Keep the enabled HAL modules consistent with stm32f1xx_hal_conf.h.
    set(HAL_SOURCE_DIR "${PROJECT_PATH}/STM32-base-STM32Cube/HAL/${SERIES_FOLDER}/src")
    set(SRC_FILES ${SRC_FILES}
            "${HAL_SOURCE_DIR}/stm32f1xx_hal.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_cortex.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_gpio.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc_ex.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_flash.c"
            "${HAL_SOURCE_DIR}/stm32f1xx_hal_flash_ex.c"
    )
endif ()

set(SOURCE_LIST ${SOURCE_LIST} ${SRC_FILES}
        "${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/src/system_stm32f1xx.c")
