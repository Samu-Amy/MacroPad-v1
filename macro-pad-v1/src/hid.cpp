#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#include "hid.hpp"
#include "device.hpp"


// ----- VARIABLES -----

// HID Descriptor
uint8_t const desc_hid_report[] = {
  
  // - Report 1: Keyboar -
  HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP),
  HID_USAGE(HID_USAGE_DESKTOP_KEYBOARD), // Per far rilevare il dispositivo come tastierino numerico si può mettere KEYPAD
  HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(1) // Define the ID (the macro already has the comma at the end)

    // Modifier keys (Ctrl, Shift, Alt, GUI — 8 bit)
    HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD),
    HID_USAGE_MIN(224), // Modifier keys (usage): 224 = LCtrl, 225 = LShift, 226 = LAlt, 227 = LGUI (Windows/Command), 228 = RCtrl, 229 = RShift, 230 = RAlt, 231 = RGUI
    HID_USAGE_MAX(231),
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX(1),
    HID_REPORT_SIZE(1), // 1 bit per field
    HID_REPORT_COUNT(8), // 8 fields * 1 bit = 8 bit (1 Byte)
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), // Variable: every field (bit) of the report correspond to a usage (modifier) (for example 00000011 -> LCtrl + LShift)

    // Reserved byte
    HID_REPORT_SIZE(8),
    HID_REPORT_COUNT(1),
    HID_INPUT(HID_CONSTANT),

    // Keycodes (6 keys simultaneously)
    HID_USAGE_PAGE(HID_USAGE_PAGE_KEYBOARD),
    HID_USAGE_MIN(0),
    HID_USAGE_MAX_N(255, 2), // 2 Bytes because hid interprets values as signed (it indicates how many bytes to use in the descriptor itself to encode the number, not in the report)
    HID_LOGICAL_MIN(0),
    HID_LOGICAL_MAX_N(255, 2),
    HID_REPORT_SIZE(8), // 8 bits per field
    HID_REPORT_COUNT(BUTTON_COUNT), // Number of Keys that can be pressed at the same time
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE), // Array: every field of the report contains the value that represent the usage (for example 0x04 in one field -> 'A' is pressed, 0x28 in another field → also "Enter" is pressed)
  
  HID_COLLECTION_END,

  // - Report 2: Consumer Controls -
  HID_USAGE_PAGE(HID_USAGE_PAGE_CONSUMER),
  HID_USAGE(HID_USAGE_CONSUMER_CONTROL),
  HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(2)

    // Controls
    HID_USAGE_MIN(0x00),
    HID_USAGE_MAX_N(0x29C, 2),
    HID_LOGICAL_MIN(0x00),
    HID_LOGICAL_MAX_N(0x29C, 2),
    HID_REPORT_SIZE(16), // 16 bits per field
    HID_REPORT_COUNT(1),
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE),

  HID_COLLECTION_END,

  // - Report 3: software communication -
  HID_USAGE_PAGE_N(HID_USAGE_PAGE_VENDOR, 2), // HID_USAGE_PAGE_N(0xFF00, 2), // TODO: se non funziona con "HID_USAGE_PAGE_VENDOR", usa il valore direttamente ("0xFF00")
  HID_USAGE(0x01),
  HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(3)

    // Input (device -> software)
    HID_USAGE(0x02),
    HID_LOGICAL_MIN(0x00),
    HID_LOGICAL_MAX_N(0xFF, 2),
    HID_REPORT_SIZE(8), // 8 bits per field
    HID_REPORT_COUNT(63), // 63 Bytes
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),
    
    // Output (software -> device)
    HID_USAGE(0x03),
    HID_LOGICAL_MIN(0x00),
    HID_LOGICAL_MAX_N(0xFF, 2),
    HID_REPORT_SIZE(8), // 8 bits per field
    HID_REPORT_COUNT(63), // 63 Bytes
    HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE),

  HID_COLLECTION_END
};

// USB HID
Adafruit_USBD_HID usbHid(
  desc_hid_report,          // descriptor HID
  sizeof(desc_hid_report),  // descriptor size
  HID_ITF_PROTOCOL_NONE,    // boot protocol (mouse/keyboard/none) -> to use mouse/keyboard in boot - since this device has a composite descriptor (3 different reports), can't use mouse/keyboard
  POLLING_INTERVAL,         // polling interval (ms)
  true                      // use endpoint OUT
);

// Reports
keyboardReport_t keyboardReport = {};
consumerReport_t controlsReport = {};
communicationReport_t communicationReport = {};
