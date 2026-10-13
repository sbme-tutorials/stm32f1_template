set(INCLUDE_LIST ${INCLUDE_LIST}
        "${ARM_DIR}/arm-none-eabi/include"
        "${PROJECT_PATH}/STM32-base/startup"
        "${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/ARM/inc"
        "${PROJECT_PATH}/STM32-base-STM32Cube/CMSIS/${SERIES_FOLDER}/inc"
        "${PROJECT_PATH}/include"
        "${PROJECT_PATH}/Lib"
        "${PROJECT_PATH}/Drivers/Rcc"
        "${PROJECT_PATH}/Drivers/Gpio"
        "${PROJECT_PATH}/Drivers/Usart"
        "${PROJECT_PATH}/BSP/Panda"
)

if(USE_HAL)
    set(INCLUDE_LIST ${INCLUDE_LIST} "${PROJECT_PATH}/STM32-base-STM32Cube/HAL/${SERIES_FOLDER}/inc")
endif ()
