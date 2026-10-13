# STM32F1 CMake template for Panda Rev1

Open this folder in CLion and build the current project target, `stm32f1-test`.
The target is **STM32F103C8T6**, Cortex-M3, **64 KB Flash / 20 KB RAM**,
running from the internal **8 MHz HSI**.

The current [main.c](src/main.c) initializes the clock, and initializes USART1. It then waits for an
ASCII character:

| Input | USART reply |
| --- | --- |
| `1` | `Hello\r\n` |
| `2` | `Welcome\r\n` |
| Other characters, including Enter/CR/LF | No reply |

The program waits for a recognized command before sending a reply. Follow the
[CLion serial connection steps below](#send-and-receive-usart-commands) to send
commands and see replies.


| Folder | Contents |
| --- | --- |
| `src/` | Application entry point, `main.c` |
| `Drivers/Gpio/`, `Drivers/Rcc/`, `Drivers/Usart/` | Custom peripheral drivers |
| `BSP/Panda/` | Board pin definitions|
| `Lib/` | Shared types, bit operations and optional utilities |
| `cmake/` | Tool paths, device mapping, build and programming rules |
| `debug/` | OpenOCD configuration and manual GDB scripts |

STM32-base supplies the reset handler and vector table.

## Included dependencies

The dependencies are checked in: configuring/building needs no downloads.
Only STM32F1 startup, linker, device and HAL files are included, together with
the shared startup/linker files and ARM CMSIS headers.

`STM32F103x8.ld` provides the C8 memory layout. Its startup and CMSIS density
group is **STM32F103xB**, so the project uses `STM32F103xB.s` and defines
`STM32F103xB`. This does **not** increase the Flash allocation to 128 KB.
`startup_common.s` and `common.ld` are necessary dependencies of these files.

Upstream license notices are preserved; see `THIRD_PARTY.md`.
These vendor versions match the STM32-base ecosystem used
in the F4 template; they are not the latest ST CubeF1 drivers.

## Tools

- [Arm GNU Toolchain](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain),
  targeting **arm-none-eabi**; GCC, GDB, objcopy and size.
- CMake **3.30+** and Ninja; CLion bundles both.
- For the recommended flash/debug workflow: **OpenOCD 0.12+**, and the Windows ST-Link USB driver.
- An alternative bundle is [STM32CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html),
  which supplies the ARM tools, ST-LINK GDB server and CubeProgrammer CLI.
  OpenOCD is a separate tool.

The CMake layout follows the STM32F4 template: `CMakeLists.txt` includes
`ArmToolchain.cmake` before `project(stm32f1-test C)`, then loads `Device.cmake`,
`SourceList.cmake` and `IncludeList.cmake`. `Device.cmake` loads
`MappedTarget.cmake`. The executable uses `${CMAKE_PROJECT_NAME}`.

Set `ARM_DIR` in `cmake/ArmToolchain.cmake` to the ARM installation directory
(the folder before `bin/`), just as in the F4 template. `ARM_TOOLCHAIN_DIR` is
then derived as `${ARM_DIR}/bin`. You can also supply `-DARM_DIR=...` when
configuring, or use the local user preset below. Reset the CMake cache when
changing compilers or editing a previously cached toolchain path.

```powershell
cmake --preset debug
cmake --build cmake-build-debug
```

The optional `release` and `debug-hal` presets use `cmake-build-release/` and
`cmake-build-debug-hal/`. The normal CLion Debug profile uses
`cmake-build-debug/`. Presets no longer generate the extra `build/` directory.
For terminal commands, CMake and Ninja must be on PATH.

## Run and debug in CLion

See **[README_CLION.md](README_CLION.md)** for the complete setup, explanations
of the `debug/` files, CMake profiles/presets and generated output files.

- Select **Panda Run (HEX)** with the ordinary host **GDB** profile and click
  **Run** to build, flash HEX and run the kit.
- Select **Panda Debug (ELF)** with your working **Panda OpenOCD** debug profile
  and click **Debug** to build, upload ELF and debug the kit.

CMake generates the run configurations in `.run/` from `cmake/clion/` templates.
Changing `project(...)` and reloading CMake updates the names in the generated
files. If CLion still shows an old Debug target, select the renamed firmware
in both **Target** and **Executable** under **Run > Edit Configurations**.
Keep your working OpenOCD debug profile: `-f debug/panda-stlink.cfg`,
project root as working directory, upload **Always**, reset command
`monitor reset halt`, and connection `localhost:3333` without persistent session.

## ST-Link V2 connection

Use the **four-pin SWD header on the Blue Pill**, not the carrier's UART port.
The Panda manual, page 27, shows this pinout for the pictured USB dongle:

| ST-Link signal | Blue Pill signal | MCU pin | Pictured dongle pin |
| --- | --- | --- | --- |
| SWDIO | SWIO / SWDIO | PA13 | 4 |
| SWCLK | SWCLK | PA14 | 2 |
| GND | GND | Ground | 6 |
| 3.3 V / target reference | 3V3 | 3.3 V rail | 8 |

Follow **signal labels** on your probe; numbering varies between ST-Link
versions. On the dongle pictured in the manual, 3.3 V is a power output;
on official probes the corresponding target-voltage reference may be an input.
Power the Panda carrier through its USB Type-B input and power switch for
normal kit use. With an already powered kit and a dongle providing 3.3 V output,
use SWDIO, SWCLK and common GND; do not tie two power outputs together.
Connect a genuine probe's target reference as its manual requires. NRST is
optional if your probe exposes a real STM32 reset signal; the supplied config
uses software reset for the four-pin connector. Do not use SWIM or a 5 V pin.

Set **BOOT0 = 0** for normal Flash boot. Enable the **TX/RX DIP switches** for
the USART example.
The template leaves PA13/PA14 assigned to SWD.

## Send and receive USART commands

First build and flash the current firmware with **Panda Run (HEX)**. If using
**Panda Debug (ELF)**, resume the MCU so it can execute the receive loop.

To use it inside CLion:

1. Connect the Panda's **USB Type-B** to your computer with a data cable,
   power the kit, and enable **TX/RX DIP switches**.
2. Install **Serial Port Monitor** under **Settings → Plugins** if needed.
3. Open **View → Tool Windows → Serial Connections**.
   [JetBrains instructions](https://blog.jetbrains.com/clion/2024/04/serial-port-monitor-for-embedded-developers/).
4. Select the CH340 COM port. Find its number in Windows
   **Device Manager → Ports (COM & LPT)**. Install the CH340 driver if no COM
   port appears.
5. Set **9600 baud, 8 data bits, no parity, 1 stop bit, no flow control**, then
   **Connect**. Use text/ASCII mode; local echo can be off.
6. Send text `1` or `2`: `1` replies with `Hello`, and `2` replies with
   `Welcome`. Both replies end with a new line. Use the terminal's Send action
   or Enter if it buffers typed input.

```text
Input:  Computer → USB → CH340 → PA10 (USART1 RX)
Output: PA9 (USART1 TX) → CH340 → USB → Computer
```

Type commands and read replies in **Serial Connections**. The normal CLion
Run console displays build/debug logs. The serial window stays quiet until
you send a recognized command.

The commands are ASCII digits (`'1'` / `'2'`), not binary bytes `0x01` / `0x02`.
Wait for each reply before sending the next command. This is a simple polling
driver with no interrupt/DMA receive queue; long pasted bursts can overrun it.


`Rcc_Init` selects HSI at 8 MHz and sets both APB prescalers to divide by one.
USART1's PCLK2 is therefore 8 MHz. `Usart1_Init` configures TX as alternate-
function push-pull and RX as an input with pull-up; STM32F1 has no F4-style
AF7 pin selection. Its rounded baud divider is `BRR = 833` (`0x341`) for 9600
baud. Update the divider if you change the APB2 clock or the baud rate.

## Flash/debug from a terminal

Set `OPENOCD_BIN_DIR` to your installed OpenOCD `bin/` directory at the top of
`cmake/Programming.cmake`. Each developer supplies their own paths; the project
does not search PATH or scan installation directories. `OPENOCD_EXECUTABLE`
is derived as `${OPENOCD_BIN_DIR}/openocd${Extension}`. An existing executable
enables `flash` and `debug-server`. `flash` programs the HEX and exits.
These path settings use the CMake cache, like `ARM_DIR`: reset the cache and
reload after editing a previously configured path, or override it with
`-DOPENOCD_BIN_DIR=C:/path/to/openocd/bin`.
If OpenOCD cannot locate its bundled scripts, also configure with
`-DOPENOCD_SCRIPTS_DIR=C:/path/to/openocd/scripts`. Both programming targets
build the firmware first. `debug-server` also programs/verifies the ELF and
halts the MCU, then keeps OpenOCD running for GDB.

```powershell
cmake --build cmake-build-debug --target flash

# Terminal 1: stays running until stopped with Ctrl+C.
cmake --build cmake-build-debug --target debug-server

# Terminal 2: attach to the already programmed MCU and set a main breakpoint.
arm-none-eabi-gdb cmake-build-debug/stm32f1-test.elf -x debug/panda-attach.gdb
# In GDB: continue
```

Optionally set `STM32_PROGRAMMER_CLI` in `cmake/Programming.cmake` to its full
installed path. It enables `flash-st`, which programs/verifies/resets the ELF
through ST-Link. Leave it empty when using only OpenOCD. A command-line override
is also supported:

```powershell
cmake --preset debug '-DSTM32_PROGRAMMER_CLI=C:/path/to/STM32_Programmer_CLI.exe'
cmake --build cmake-build-debug --target flash-st
```

Programming replaces existing kit firmware, including the factory demo.

## Panda peripheral map

From the manual's schematics and pin tables (pages 10-24):

| Peripheral | Pins / notes |
| --- | --- |
| Red1, Red2, Green | PA2, PA1, PC15; active high, individual DIP switches |
| Blue, Yellow | PC14, PC13; active high, individual DIP switches |
| Buzzer | PB4; release this JTAG pin while keeping SWD before use |
| Relay1, Relay2 | PB5, PA3 |
| Analog potentiometer | PA0 |
| USART1 / CH340 | PA9 TX, PA10 RX; enable TX/RX DIP switches |
| Keypad columns 1-4 | PB11, PB10, PB1, PB0; active-low inputs with pull-ups |
| Keypad rows 1-4 | PA4, PA5, PA6, PA7; select MATRIX mode for all 16 keys |
| LCD RS, EN, D4-D7 | PA11, PA8, PB15, PB14, PB13, PB12 |
| Seven-segment A, B, C, D, E, F, G, DOT | PA11, PA8, PB15, PB14, PB13, PB12, PA12, PA15 |
| Seven-segment COM1, COM2, COM3 | PB8, PB9, PB3 |
| OLED | SCL **PB6**, SDA **PB7**, as drawn on page 22 and labeled on the PCB |

**Manual inconsistencies:** the general-output prose on page 16 lists different
pins from the schematic and PCB photo on page 15. The template follows the
schematic/photo: Red1 is PA2, Red2 PA1, Green PC15, Blue PC14, Yellow PC13,
Buzzer PB4, Relay1 PB5 and Relay2 PA3. Check your PCB labels if the kit revision
differs. The Blue Pill's own PC13 LED may also respond when driving Yellow.
Green/Blue conflict with an enabled LSE crystal on PC15/PC14.

The OLED table on page 21 reverses SCL/SDA relative to the schematic/photo;
confirm the board revision before writing its driver. LCD and seven-segment
share pins: disable the other display's DIP switches. PA11/PA12 overlap USB.
PB3/PB4/PA15 default to JTAG: disable **JTAG only, keeping SWD** when using them
(`__HAL_AFIO_REMAP_SWJ_NOJTAG()` with HAL and the AFIO clock enabled). The
USART1 example uses the default PA9/PA10 mapping and needs no USART remapping.

The carrier's **USB Type-B connects to CH340 UART**, not MCU USB debugging.
Use that COM port with your serial terminal for this example's USART1 commands.
