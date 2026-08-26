## Nova Desk firmware

This is the base of the firmware, the starting point of the project Nova Desk (it will be expanded to an ecosystem of different devices, starting from this [Macro]Pad v1).

The _firmware_ (C++) uses:

- TinyUSB (for usb communication and device recognition from OS)
- U8g2 (oled display)

The _software_ (C++) uses:

- [Hidapi](https://github.com/libusb/hidapi) (for usb devices scan/connection and communication)
- Qt QML (for the interface)

---

## Setup

This are the steps to do to setup the firmware (you can modify your version of the firmware to adapt it to a macropad like (or similar to) this one or your own custom device, in every case there are some things to check before uploading it in order to have a working device):

1. Define pins (encoder and matrix (MatrixBtn)) in `inputs.hpp`
2. If needed, modify the u8g2 initialization in `display.cpp`
3. If needed, update the hid reports and description in `hid.cpp`

---

## Notes

- When something in the Config structure change (a field is moved or is created/deleted or when a const value used for the arrays is changed), you need to modify `CONFIG_VERSION' in order to be sure that the defaultConfig is loaded again after the change (the crc32 is used to check the validity of the saved config, if for some reason the value is valid even with the changes, the defaultConfig wouldn't be updated)
