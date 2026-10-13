# Panda: Run, Debug, CMake profiles and output folders

Use **Panda Run (HEX)** with the green **Run** button to build, flash the HEX,
reset the MCU and let it run. Use **Panda Debug (ELF)** with the **Debug** button
and the **Panda OpenOCD** debug profile to build, flash the ELF and debug it.

These are two run configurations in the toolbar's configuration selector.
Select the one for the action you want. The supplied configurations do not
switch their executable automatically when you change between Run and Debug.

For the current USART `main.c`, see the
[serial connection steps in README.md](README.md#send-and-receive-usart-commands).
Use **Serial Connections** to send `1`/`2` and receive `Hello`/`Welcome`.

## One-time setup in your existing CLion project

**Where to find Panda Run and Panda Debug:** open **Run > Edit Configurations**
(or press **Alt+Shift+F10**, then **0**). They are run configurations, so they
are not listed under **Settings > CMake**. In the main toolbar, open the
configuration selector that shows your firmware target (`stm32f1-test`) and choose
**Panda Run (HEX)** or **Panda Debug (ELF)**. The separate `Debug | ...` selector
chooses the CMake build profile and debugger profile.

The generated configuration files are `.run/Panda_Run_HEX.run.xml` and
`.run/Panda_Debug_ELF.run.xml`. CMake creates them when configuring the project.
If they are missing from **Run > Edit
Configurations**, close and reopen this project so CLion reads the shared
configuration files.

[JetBrains describes the Run > Edit Configurations dialog here](https://www.jetbrains.com/help/clion/run-debug-configuration.html).

1. Reload the CMake project after these changes. CLion loads the shared run
   configurations from `.run/`. If they do not appear, reopen the project.
2. In **Settings > Build, Execution, Deployment > CMake**, keep your normal
   **Debug** profile enabled, with build directory `cmake-build-debug` and
   build type `Debug`. The optional preset profiles can remain disabled.
3. Select **Panda Debug (ELF)**, then select your working **Panda OpenOCD**
   debug profile in the toolbar. This selection may need to be made once for
   the new run configuration.
4. Keep the OpenOCD profile settings shown below. Set a breakpoint in `main.c`
   and click **Debug**. To run freely after flashing HEX, stop debugging,
   stop the Panda OpenOCD server in **Services**, select **Panda Run (HEX)**
   with the ordinary host **GDB** profile, and click **Run**. Keep **Panda
   OpenOCD** selected for **Panda Debug (ELF)**.

The ARM toolchain configuration and its variables have not been changed.
Each developer sets their tool paths at the top of `cmake/Programming.cmake`:

```cmake
set(OPENOCD_BIN_DIR "C:/Tools/xpack-openocd-0.12.0-7/bin"
    CACHE PATH "OpenOCD installation bin directory")
set(OPENOCD_EXECUTABLE "${OPENOCD_BIN_DIR}/openocd${Extension}")
set(STM32_PROGRAMMER_CLI ""
    CACHE FILEPATH "Full path to STM32_Programmer_CLI; leave empty when unused")
```

Edit `OPENOCD_BIN_DIR`; the executable path is derived using the toolchain's
`${Extension}` variable (`.exe` on Windows). There is no tool search or
installation-directory scan. Leave
`STM32_PROGRAMMER_CLI` empty when using only OpenOCD. As with `ARM_DIR`, these
settings are cached: after editing a previously configured path in the file,
use **Reset Cache and Reload Project**. You can also supply paths in a local
user preset or through the active CMake profile's options:

```text
-DOPENOCD_BIN_DIR=C:/Tools/xpack-openocd-0.12.0-7/bin
```

## The two button workflows

| Selection and button | What happens |
| --- | --- |
| **Panda Run (HEX)** + **Run**, with host **GDB** profile selected | Runs CMake's `flash` target. It builds the firmware, programs the current project's HEX, verifies it, resets/runs the MCU and exits OpenOCD. |
| **Panda Debug (ELF)** + **Debug**, using **Panda OpenOCD** | Builds the current project's ELF, launches OpenOCD, connects GDB and uploads the ELF because upload is set to **Always**. The debugger then controls the MCU. |

Run uses CLion's `$CMakeCurrentExecutableName$` and `$CMakeCurrentBuildDir$`
macros to select CMake and the active profile's build directory. Its command is:

```text
--build "$CMakeCurrentBuildDir$" --target flash
```

Run selects the CMake `flash` target, which depends on `${EXECUTABLE}` and
rebuilds changed source files and regenerates the HEX before programming.
Debug selects `${EXECUTABLE}`, whose executable is the ELF. Both paths follow
the selected CMake build profile.

### Renaming the template project

The editable configuration templates are in `cmake/clion/`. Their selectors
use `@CMAKE_PROJECT_NAME@` and `@EXECUTABLE@`; `cmake/Clion.cmake` substitutes
these values with `configure_file(... @ONLY)` on every CMake configuration.
CLion's `$...$` path macros remain intact.

For example, change `project(stm32f1-test C)` to `project(my-controller C)` in
`CMakeLists.txt`, then reload CMake. `${EXECUTABLE}` already comes from
`${CMAKE_PROJECT_NAME}`, so Debug selects `my-controller` and the firmware
files become `my-controller.elf`, `.hex`, `.bin` and `.map`. Run still selects
`flash`, which obtains the firmware path from the CMake target.

The generated `.run/` XML will contain literal resolved names, as required by
CLion. Edit the templates under `cmake/clion/` rather than the generated files;
CMake regenerates `.run/`, which is ignored by Git. This updates the files on
disk; CLion can retain the old selectors in an already loaded configuration,
especially while **Run > Edit Configurations** is open. A CMake reload does
not guarantee that the active run-configuration editor refreshes too.

If Debug shows the old target with **(Not found)** after a rename:

1. In **Run > Edit Configurations > Panda Debug (ELF)**, select the new
   executable target in **both Target and Executable**, then click **Apply**.
   For the current `project(stm32f1-test C)`, select `stm32f1-test` in both.
2. Keep the **Build** before-launch step enabled and use the working
   **Panda OpenOCD** debug profile.

Alternatively, cancel the configuration dialog and reopen the project to load
the generated configurations from disk. If closing the project saves an old
configuration over a generated file, reload CMake to regenerate it and select
the current target in the two dropdowns above.

Run continues to work because its target is always `flash` and its executable
is CMake. Debug refers to a firmware target whose name changes with `project(...)`.

Do not use **Panda Run (HEX)** for debugging: its executable is the host CMake
program. **Panda Debug (ELF)** is the configuration whose executable is the MCU
firmware.

## Your working Panda OpenOCD debug profile

Under **Settings > Build, Execution, Deployment > Debugger > Debug Profiles**,
keep the existing **GDB Server** profile named **Panda OpenOCD**:

| Tab | Setting | Value |
| --- | --- | --- |
| GDB Server | Use external GDB server | Unchecked |
| GDB Server | Executable | `C:\Tools\xpack-openocd-0.12.0-7\bin\openocd.exe` |
| GDB Server | Arguments | `-f debug/panda-stlink.cfg` |
| GDB Server > Advanced | Working directory | This project's root directory, containing `CMakeLists.txt` and `debug/` |
| Device Settings | Upload executable to device | **Always** |
| Device Settings | Console | Enabled, port **4444** |
| Device Settings | Reset command | `monitor reset halt` |
| Device Settings | Before upload / After upload | Both checked |
| Debugger | Toolchain | Automatic is sufficient for choosing the build environment. |
| Debugger | Executable | Explicitly choose `C:\Tools\arm-gnu-toolchain-15.3.rel2\bin\arm-none-eabi-gdb.exe`. Do not leave this on Auto if it chooses CLion's desktop GDB. |
| Debugger > Connection | Persistent session | Unchecked |
| Debugger > Connection | Arguments | `localhost:3333` |
| Debugger > Script | Automatic | `target remote localhost:3333` |

OpenOCD provides access to the STM32 through ST-Link. GDB uses the ELF's symbols
to connect machine instructions to C source lines, variables and breakpoints.
CLion's upload setting programs the selected ELF; no separate `program ... exit`
command is needed in the debug profile. Leave this server running throughout
the debug session. The CMake `debug-server` target is a separate terminal
alternative; it is not a before-launch step for this profile.

[JetBrains documents debug-server profiles and their toolbar selection here](https://www.jetbrains.com/help/clion/debug-servers.html).

## If Run starts GDB or flashing reports USB access errors

A HEX Run should start `cmake.exe --build ... --target flash` and then OpenOCD.
It should not start a GDB session or send `monitor reset halt` commands through
GDB. Under **Run > Edit Configurations > Panda Run (HEX)**, keep **Target** set
to `flash`, **Executable** set to `$CMakeCurrentExecutableName$`, and
**Program arguments** set to `--build "$CMakeCurrentBuildDir$" --target flash`.
Select the ordinary **GDB** profile for this host command and click the **Run**
triangle. For debugging, select **Panda Debug (ELF)** and **Panda OpenOCD**.

`Selected architecture i386:x86-64 is not compatible ... arm` and
`Truncated register ... in remote 'g' packet` mean the desktop debugger cannot
interpret the ARM target's register layout. In **Debug Profiles > Panda
OpenOCD > Debugger > Executable**, explicitly select the toolchain's
`arm-none-eabi-gdb.exe`. The debugger runs on Windows, but its target must be
ARM; `--configuration` should report `--target=arm-none-eabi`.
If an external edit changed the saved profile while CLion was open, set the
same executable in CLion's UI and click **Apply** to update its loaded state.

`LIBUSB_ERROR_ACCESS` can occur when another OpenOCD/ST-Link session still owns
the probe. Stop the failed debug session and its OpenOCD server in **Services**
before running `flash`. Stopping only GDB can leave its server alive. Use
**Persistent session: unchecked**, as above. If access still fails after all
probe users have stopped, reconnect ST-Link and check its USB driver.

[CLion documents debugger profile selection and explicit executable paths here](https://www.jetbrains.com/help/clion/debug-profiles.html).

## What is inside `debug/`?

This folder contains **configuration scripts**, not compiled firmware.

| File | Purpose | Used by the normal CLion workflow? |
| --- | --- | --- |
| `panda-stlink.cfg` | OpenOCD board setup: ST-Link over SWD, STM32F1 target, 100 kHz adapter speed, software reset without an NRST wire. | **Yes**, for Run and Debug. |
| `panda.gdb` | Manual GDB commands: connect to a separately started OpenOCD server, halt/reset, upload ELF with `load`, reset/halt and break at `main`. | No; for terminal use. |
| `panda-attach.gdb` | Manual GDB commands to attach to firmware already uploaded by CMake's `debug-server`, then break at `main`. It does not upload again. | No; for terminal use. |

OpenOCD's default GDB port is **3333**. Port **4444** is its command console;
it is not the firmware's UART. `adapter speed 100` means **100 kHz SWD speed**,
not the CPU's clock. The firmware currently uses the 8 MHz HSI clock.

## What are profiles, and how does CMake see them?

There are three different selections:

| Item | What it controls | Example |
| --- | --- | --- |
| **CMake build profile** | Compiler environment, CMake options, build type and output directory. | `Debug` |
| **Run configuration** | The target/executable and command to start when you press a button. | `Panda Run (HEX)` or `Panda Debug (ELF)` |
| **Debug profile** | GDB, OpenOCD connection and firmware upload/reset settings. | `Panda OpenOCD` |

A CLion CMake profile name is an IDE label. CMake receives its actual settings,
such as `-DCMAKE_BUILD_TYPE=Debug`, `-DUSE_HAL=OFF`, the generator and the output
directory (`-B cmake-build-debug`). CMake evaluates `CMakeLists.txt` with these
values, saves configuration in that directory's `CMakeCache.txt`, and generates
the Ninja build rules. Ninja then compiles and links the firmware.

`CMakePresets.json` is a shared list of named CMake settings. CLion imports
presets into CMake profiles; terminal CMake uses one when you explicitly run
`cmake --preset debug`. Your ordinary CLion `Debug` profile does not inherit
the `debug` preset simply because the names are similar.

| Profile/preset | Output directory | Settings and purpose |
| --- | --- | --- |
| Normal CLion `Debug` | `cmake-build-debug` | Your existing daily development profile. Use build type `Debug`. |
| `debug`: Panda - Debug (CMSIS) | `cmake-build-debug` | `Debug`, `USE_HAL=OFF`. Optional preset equivalent for this template. |
| `release`: Panda - Release (CMSIS) | `cmake-build-release` | `Release`, `USE_HAL=OFF`; size optimization. |
| `debug-hal`: Panda - Debug (HAL available) | `cmake-build-debug-hal` | `Debug`, `USE_HAL=ON`; makes selected HAL modules available. `main.c` still uses your drivers. |

Use either the normal **Debug** profile or the **debug** preset profile for
the same output directory. Keep one enabled for daily use. Other preset
profiles are optional; grey entries in your screenshot are disabled.

The old JSON also contained three **build presets**. CLion imported both the
configure presets and build presets, producing entries such as
`debug - debug preset`. Those redundant build presets have been removed.
After reloading presets, CLion should remove those three imported entries.
If they remain visible, use **Load CMake Presets** or reopen the project.

For your own paths, copy `CMakeUserPresets.json.example` to
`CMakeUserPresets.json` and edit it. Its optional `local-debug` preset uses
`cmake-build-local-debug`. It is not needed with your current working setup.

[CLion's preset import behavior](https://www.jetbrains.com/help/clion/cmake-presets.html) and
[CMake's preset format](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html) describe these settings.

## Where are the output files?

Presets now use only `cmake-build-*` directories, matching CLion's naming
convention. Your existing `cmake-build-debug/` directory is retained.
The obsolete root `build/` directory and the extra Release/HAL verification
directories are now absent. The optional Release/HAL directories are created
only when those profiles are configured. The updated presets will not recreate
the old root `build/` directory.

Verification should run in the foreground. Stop any processes started just for
verification and remove their temporary output directories afterward. Keep
the active `cmake-build-debug/` directory used for your normal development.

For the normal Debug profile:

| File under `cmake-build-debug/` | Purpose |
| --- | --- |
| `stm32f1-test.elf` | Linked firmware with debug information; used for source debugging and ELF upload. |
| `stm32f1-test.hex` | Intel HEX firmware with addresses; used by the Run flashing target. |
| `stm32f1-test.bin` | Raw firmware bytes; available for programmers that require BIN. |
| `stm32f1-test.map` | Linker report showing where code/data occupy Flash and RAM. |
| `CMakeCache.txt`, `build.ninja`, `CMakeFiles/` | Generated configuration, build rules and object files. |
| `compile_commands.json` | Compiler command database used by code analysis tools. |

HEX and ELF come from the same build and contain the same application.
HEX does not contain the source debugging information that GDB needs.
Generated build directories can be recreated by configuring and building.
Keep the `debug/` scripts and `cmake/clion/` templates as project source files.
CMake regenerates the `.run/` configurations from those templates.

## Equivalent terminal commands

With CMake and Ninja on PATH, from the project root:

```powershell
cmake --preset debug
cmake --build cmake-build-debug

# Build, flash HEX, verify, reset/run, and exit OpenOCD.
cmake --build cmake-build-debug --target flash

# Optional terminal debugging instead of the CLion OpenOCD profile:
# Terminal 1: build/upload ELF, halt, and keep the server running.
cmake --build cmake-build-debug --target debug-server

# Terminal 2: attach to that server using the same build's ELF.
arm-none-eabi-gdb cmake-build-debug/stm32f1-test.elf -x debug/panda-attach.gdb
# In GDB: continue
```

Stop the current debug server before starting another action that uses ST-Link.
