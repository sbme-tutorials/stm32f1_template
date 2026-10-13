# Usage: arm-none-eabi-gdb cmake-build-debug/stm32-test.elf -x debug/panda.gdb
# Start OpenOCD using panda-stlink.cfg first. This script writes firmware.
set pagination off
target extended-remote localhost:3333
monitor reset halt
load
monitor reset halt
break main
