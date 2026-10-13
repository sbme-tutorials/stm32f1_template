# CLion's target selectors contain literal names, not CMake/IDE macros.
# Generate them from the current project and firmware target at configure time.
# @ONLY keeps CLion's $...$ path macros intact.
foreach(CLION_CONFIGURATION IN ITEMS Panda_Run_HEX Panda_Debug_ELF)
    configure_file(
        "${PROJECT_PATH}/cmake/clion/${CLION_CONFIGURATION}.run.xml.in"
        "${PROJECT_PATH}/.run/${CLION_CONFIGURATION}.run.xml"
        @ONLY
    )
endforeach()
