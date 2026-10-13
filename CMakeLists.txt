cmake_minimum_required(VERSION 3.30)
include(cmake/ArmToolchain.cmake)

# Report missing tools before CMake starts its compiler checks.
foreach(ARM_TOOL IN ITEMS CMAKE_C_COMPILER CMAKE_OBJCOPY CMAKE_SIZE)
    if(NOT EXISTS "${${ARM_TOOL}}")
        message(FATAL_ERROR
                "${ARM_TOOL} was not found at '${${ARM_TOOL}}'. "
                "Set ARM_DIR to the toolchain installation folder before bin "
                "in cmake/ArmToolchain.cmake or with -DARM_DIR=...")
    endif()
endforeach()

project(stm32f1-test C)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS        ON)

set(CMAKE_C_STANDARD                11)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(CMAKE_C_EXTENSIONS              ON)
enable_language(ASM)

set(PROJECT_PATH                         "${CMAKE_CURRENT_SOURCE_DIR}")
include(cmake/Device.cmake)
include(cmake/SourceList.cmake)
include(cmake/IncludeList.cmake)

set(LINKER_SCRIPTS_BASE_DIR              "${PROJECT_PATH}/STM32-base/linker")
set(STARTUP_BASE_DIR                     "${PROJECT_PATH}/STM32-base/startup")

set(EXECUTABLE                           ${CMAKE_PROJECT_NAME})

# Keep artifacts separate when using a generator such as Ninja Multi-Config.
set(OUTPUT_DIR                          "${CMAKE_CURRENT_BINARY_DIR}")
if(CMAKE_CONFIGURATION_TYPES)
    set(OUTPUT_DIR                      "${OUTPUT_DIR}/$<CONFIG>")
endif()

# Executable files
add_executable(${EXECUTABLE} ${SOURCE_LIST} "${STARTUP_BASE_DIR}/${SERIES_FOLDER}/${MAPPED_DEVICE}.s")
set_source_files_properties("${STARTUP_BASE_DIR}/${SERIES_FOLDER}/${MAPPED_DEVICE}.s"
        PROPERTIES OBJECT_DEPENDS "${STARTUP_BASE_DIR}/startup_common.s")
# Include
target_include_directories(${EXECUTABLE} PUBLIC ${INCLUDE_LIST})
# Project symbols
target_compile_definitions(${EXECUTABLE} PUBLIC ${COMPILER_DEFINE})
# Compiler options
target_compile_options(${EXECUTABLE} PUBLIC
        ${CPU_PARAMETERS}
        -Wall
        -Wpedantic
        -Wno-unused-parameter
        # startup_common.s uses assembler .ifdef, so define its symbol in GAS.
        "$<$<COMPILE_LANGUAGE:ASM>:-Wa,--defsym,CALL_ARM_SYSTEM_INIT=1>"
)

# Linker options
target_link_options(${EXECUTABLE} PRIVATE
        ${CPU_PARAMETERS}
        "-L${LINKER_SCRIPTS_BASE_DIR}"
        "-T${LINKER_SCRIPTS_BASE_DIR}/${SERIES_FOLDER}/${DEVICE}.ld"
        -Xlinker --gc-sections
        "-Wl,-Map=${OUTPUT_DIR}/${CMAKE_PROJECT_NAME}.map"
        -Wl,--print-memory-usage
        --specs=nano.specs
        --specs=nosys.specs
        #-u _printf_float                # STDIO float formatting support
)
set_property(TARGET ${EXECUTABLE} APPEND PROPERTY LINK_DEPENDS
        "${LINKER_SCRIPTS_BASE_DIR}/${SERIES_FOLDER}/${DEVICE}.ld"
        "${LINKER_SCRIPTS_BASE_DIR}/common.ld"
)

# Execute post-build to print size
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${EXECUTABLE}>"
        BYPRODUCTS "${OUTPUT_DIR}/${EXECUTABLE}.map"
        VERBATIM
)
# Convert output to hex and binary
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O ihex "$<TARGET_FILE:${EXECUTABLE}>"
                "$<TARGET_FILE_DIR:${EXECUTABLE}>/${EXECUTABLE}.hex"
        BYPRODUCTS "${OUTPUT_DIR}/${EXECUTABLE}.hex"
        VERBATIM
)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${EXECUTABLE}>"
                "$<TARGET_FILE_DIR:${EXECUTABLE}>/${EXECUTABLE}.bin"
        BYPRODUCTS "${OUTPUT_DIR}/${EXECUTABLE}.bin"
        VERBATIM
)

include(cmake/Programming.cmake)
include(cmake/Clion.cmake)
