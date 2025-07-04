# MicroPython

Pico-ice and pico2-ice support [MicroPython](https://micropython.org/).

## To install

- Acquire the [latest release](https://github.com/tinyvision-ai-inc/pico-ice-micropython/releases/latest) for the device you have.
- Program this `.uf2` file into the RP2350 as documented [here](programming_the_mcu.md).

## To use

The API to use the FPGA is as follow:

- `ice` is the microypthon module: `import ice`
- `ice.fpga` is the class provided by the module to manage the ICE40 FPGA: `ice.fpga(...)`
- `ice.fpga` provides the methods `stop()`, `start()`, and `cram(file)`

## How to program a bitstream to the FPGA RAM

- The `Pin` class is imported from the machine module to provide interfacing with the hardware.
- The module is imported
- The fpga type is configured with the approriate pins for the device, as well as the frequency (in MHz) that the FPGA will run at.
- The bitstream file is opened, in byte (as opposed to text) mode (`b`), to read (`r`).
- The fpga is brought out of reset with `fpga.start()`
- The bitstream is loaded into the FPGA using `fpga.cram(file)` with the previously opened file as argument.

### On pico-ice:

Make sure to adjust `frequency=` to the same frequency used by your FPGA design.
Often a lower value such as 12 MHz.

```python
from machine import Pin
import ice
fpga = ice.fpga(cdone=Pin(26), clock=Pin(24), creset=Pin(27), cram_cs=Pin(9), cram_mosi=Pin(8), cram_sck=Pin(10), frequency=48)
file = open("bitstream.bin", "br")
fpga.start()
fpga.cram(file)
```

### On pico2-ice:

Make sure to adjust `frequency=` to the same frequency used by your FPGA design.
Often a lower value such as 12 MHz.

```python
from machine import Pin
import ice
fpga = ice.fpga(cdone=Pin(40), clock=Pin(21), creset=Pin(31), cram_cs=Pin(5), cram_mosi=Pin(4), cram_sck=Pin(6), frequency=48)
file = open("bitstream.bin", "br")
fpga.start()
fpga.cram(file)
```

## How to program a bistream to the FPGA Flash (persistent bitstream)

### On pico-ice:

Make sure to adjust `frequency=` to the same frequency used by your FPGA design.
Often a lower value such as 12 MHz.

```python
from machine import Pin
import ice
file = open("bitstream.bin", "br")
flash = ice.flash(miso=Pin(8), mosi=Pin(11), sck=Pin(10), cs=Pin(9))
flash.erase(4096) # Optional
flash.write(file)
# Optional
fpga = ice.fpga(cdone=Pin(26), clock=Pin(24), creset=Pin(27), cram_cs=Pin(9), cram_mosi=Pin(8), cram_sck=Pin(10), frequency=48)
fpga.start()
```

### On pico2-ice:

Make sure to adjust `frequency=` to the same frequency used by your FPGA design.
Often a lower value such as 12 MHz.

```python
from machine import Pin
import ice
file = open("bitstream.bin", "br")
flash = ice.flash(miso=Pin(4), mosi=Pin(7), sck=Pin(6), cs=Pin(5))
flash.erase(4096) # Optional
flash.write(file)
# Optional
fpga = ice.fpga(cdone=Pin(40), clock=Pin(21), creset=Pin(31), cram_cs=Pin(5), cram_mosi=Pin(4), cram_sck=Pin(6), frequency=48)
fpga.start()
```
