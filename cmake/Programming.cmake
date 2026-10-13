# Set your installed tool paths here, following ArmToolchain.cmake's convention.
set(OPENOCD_BIN_DIR "C:/Tools/xpack-openocd-0.12.0-7/bin"
    CACHE PATH "OpenOCD installation bin directory")
set(OPENOCD_EXECUTABLE "${OPENOCD_BIN_DIR}/openocd${Extension}")
# Optional: set the full STM32_Programmer_CLI path to enable flash-st.
set(STM32_PROGRAMMER_CLI ""
    CACHE FILEPATH "Full path to STM32_Programmer_CLI; leave empty when unused")

set(OPENOCD_CONFIG "${PROJECT_PATH}/debug/panda-stlink.cfg" CACHE FILEPATH
    "OpenOCD board configuration")
set(OPENOCD_SCRIPTS_DIR "" CACHE PATH "Optional directory containing the OpenOCD scripts")

# Missing programming tools never prevent a firmware build.
if(OPENOCD_EXECUTABLE AND EXISTS "${OPENOCD_EXECUTABLE}")
    if(NOT EXISTS "${OPENOCD_CONFIG}")
        message(FATAL_ERROR "OPENOCD_CONFIG does not exist: ${OPENOCD_CONFIG}")
    endif()
    set(OPENOCD_ARGUMENTS -f "${OPENOCD_CONFIG}")
    if(OPENOCD_SCRIPTS_DIR)
        if(NOT IS_DIRECTORY "${OPENOCD_SCRIPTS_DIR}")
            message(FATAL_ERROR "OPENOCD_SCRIPTS_DIR is not a directory: ${OPENOCD_SCRIPTS_DIR}")
        endif()
        list(PREPEND OPENOCD_ARGUMENTS -s "${OPENOCD_SCRIPTS_DIR}")
    endif()
    add_custom_target(flash
        COMMAND "${OPENOCD_EXECUTABLE}" ${OPENOCD_ARGUMENTS}
            -c "program {$<TARGET_FILE_DIR:${EXECUTABLE}>/${EXECUTABLE}.hex} verify reset exit"
        WORKING_DIRECTORY "${PROJECT_PATH}"
        DEPENDS ${EXECUTABLE} USES_TERMINAL VERBATIM)
    add_custom_target(debug-server
        COMMAND "${OPENOCD_EXECUTABLE}" ${OPENOCD_ARGUMENTS}
            -c "program {$<TARGET_FILE:${EXECUTABLE}>} verify"
            -c "reset halt"
        WORKING_DIRECTORY "${PROJECT_PATH}"
        DEPENDS ${EXECUTABLE}
        USES_TERMINAL VERBATIM)
else()
    message(STATUS "Set OPENOCD_BIN_DIR in cmake/Programming.cmake to the directory containing openocd${Extension} to enable flash/debug-server targets")
endif()

if(STM32_PROGRAMMER_CLI AND EXISTS "${STM32_PROGRAMMER_CLI}")
    add_custom_target(flash-st
        COMMAND "${STM32_PROGRAMMER_CLI}" -c port=SWD
            -w "$<TARGET_FILE:${EXECUTABLE}>" -v -rst
        DEPENDS ${EXECUTABLE} USES_TERMINAL VERBATIM)
elseif(STM32_PROGRAMMER_CLI)
    message(STATUS "STM32_PROGRAMMER_CLI does not exist: check its path in cmake/Programming.cmake")
endif()
