## Nova Desk

This is the starting point of the project (it will be expanded to an ecosystem of different devices, starting from this [Macro]Pad v1).

The *firmware* (C++) uses:
- TinyUSB (for usb communication and device recognition from OS)
- U8g2 (oled display)

The *software* (C++) uses:
- Hidapi (for usb devices scan/connection and communication)
- Slint (for the interface)
