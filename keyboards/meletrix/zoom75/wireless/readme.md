# meletrix/zoom75/wireless

![meletrix/zoom75/wireless](imgur.com image replace me!)

Another firmware option for the Zoom75 Wireless PCB. This firmware will turn the keyboard into a wired-only variant.

* Keyboard Maintainer: [Mark Hooijberg](https://github.com/MHooijberg)
* Hardware Supported: WQ206A1UBTG1 V1.12 1723 KB81 PCB
* Hardware Availability: [The Official Product Page](https://meletrix.com/products/zoom75-collection)

Make example for this keyboard (after setting up your build environment):

    make meletrix/zoom75/wireless:default

Flashing example for this keyboard:

    make meletrix/zoom75/wireless:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard
* **Physical reset button**: Briefly press the button on the back of the PCB - some may have pads you must short instead
* **Keycode in layout**: Press the key mapped to `QK_BOOT` if it is available
