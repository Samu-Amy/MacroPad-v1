# Nova Desk firmware

This is the base of the firmware, used by the [Macro]Pad v1.

---

## Setup

This are the steps to do to setup the firmware (you can modify the firmware to adapt it to a macropad like (or similar to) this one or your own custom device, in every case there are some things to check before uploading it in order to have a working device):

1. Define pins (encoder and matrix (MatrixBtn)) in `inputs.hpp`
2. If needed, modify the u8g2 initialization in `display.cpp`
3. If needed, update the hid reports and description in `hid.cpp`

---

## Notes

- When something in the Config structure change (a field is moved or is created/deleted or when a const value used for the arrays is changed), you need to modify `CONFIG_VERSION' in order to be sure that the defaultConfig is loaded again after the change (the crc32 is used to check the validity of the saved config, if for some reason the value is valid even with the changes, the defaultConfig wouldn't be updated)
