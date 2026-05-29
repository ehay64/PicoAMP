# PicoAMP
A minimal example of asymmetric multiprocessing with Zephyr on the RP2040
microcontroller.

## Purpose
To demonstrate asymmetric multicore processing (AMP) on the RP2040
microcontroller by running two different Zephyr applications at the same time.
Also, to act as a testbed for AMP capabilities in Zephyr.

## How
On proper AMP microcontrollers I've worked with before, each processor complex
has its own main memory that can be isolated from all other cores. With the
RP2040, one has to be careful since both cores share the same flash and RAM
address space. This means that each core could easily clobber the main memory of
the other.

To get around this, both the flash and the RAM have been divided in half. The
first halves are assigned to core 0, while the second halves are assigned to
core 1. This means that each core has 1MB of flash and 128KB of RAM to work
with.

Isolating the two halves for each core is done by the linker and the linker
scripts. By defining where the `.text`, `.data`, `.bss`, and other sections go,
this effectively bounds each core to its own half. Practically though, nothing
is stopping one core from accessing the contents of the other core.

The usual warnings about sharing peripherals, and other on chip resources, apply
to the R2040 as they do to a proper AMP microcontroller. Don't look here if you
want advice on how to do this! Multithreading concepts apply.

## Building
Each application must be built separately. The following commands can be used to
build the applications:

```
$ source path/to/your/zephyr/venv/bin/activate
$ mkdir core_0/build
$ cmake -GNinja -DBOARD=rpi_pico core_0/ -B core_0/build/
$ ninja -C core_0/build/
$ mkdir core_1/build
$ cmake -GNinja -DBOARD=rpi_pico core_1/ -B core_1/build/
$ ninja -C core_1/build/
```

Maybe this can be automated with `west`, I don't know.

## Running
Each application is loaded and executed with the debugger. With the VSCode
launch configurations provided here, it is expected that OpenOCD is running in
a separate terminal. It can be launched with the following command:

```
$ openocd \
 -s path/to/openocd/scripts \
 -f interface/jlink.cfg \
 -c "set USE_CORE 3" \
 -f target/rp2040.cfg \
 -c "adapter speed 1000" \
 -c "transport select swd"
```

Each application can then be launched from the debug UI in VSCode. But, when
running both applications from the debugger, it's best to launch core 0, wait
for it to break on `main()`, then launch core 1. This is because both cores need
to be halted when writing to flash. If one of the cores is running while
attempting to load the program for the other core, the debugger will halt and
reset the running core - at least with OpenOCD, I'm not sure about other debug
servers like J-Link GDB Server.
