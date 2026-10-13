# Usage: arm-none-eabi-gdb cmake-build-debug/stm32-test.elf -x debug/panda-attach.gdb
# Start the CMake debug-server target first; it builds and programs the ELF.
set pagination off
target remote localhost:3333
monitor reset halt
break main
