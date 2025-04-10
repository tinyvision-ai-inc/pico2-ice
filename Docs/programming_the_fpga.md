# Programming the iCE40

The FPGA normally boots from a dedicated serial NOR flash.
This flash can be programmed by the Pico processor which exposes the flash to the host OS as either a removable drive or a DFU endpoint.

On Windows, while the RaspberryPi guide mentions using Visual Studio Code with a plugin a an IDE, better results were obtained by using the [WSL2 environment](https://learn.microsoft.com/en-us/windows/wsl/install).


## With Micropython

Out of the box, pico2-ice comes with a micropython firmware loaded in the RP2350, you can use it to upload bitstreams to the FPGA:

1. Copy bitstream file to the USB MSC disk provided by micropython, unplug and replug the pico2-ice. Alternatively, use mpremote to copy the file `mpremote cp file.bin :`

2. See the usage of micropython to upload your bitstream to FPGA flash or FPGA RAM: [How to use Micropython](md_mpy.html)

## Troubleshooting


### Checking the CDONE pin

Once the FPGA bitfile transfers over using the DFU protocol,
the Pico will check for whether the DONE pin goes high indicating a successful boot.
This would make the CDONE green LED bright.
If this doesnt happen for whatever reason,
the DFU utility will throw an error indicating that this did not succeed.


### Booting the FPGA with custom firmware

The user writing a custom firmware with the pico-ice-sdk should take care of starting the FPGA from the MCU.
Review the [pico_fpga](https://github.com/tinyvision-ai-inc/pico-ice-sdk/tree/main/examples/pico_fpga) example
for how this can be done.
