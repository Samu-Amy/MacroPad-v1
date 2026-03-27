#include <Arduino.h>
#include "headers/hid.h"

#include "headers/device.h"


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
    HID_REPORT_SIZE(16), // 2 Bytes
    HID_REPORT_COUNT(1),
    HID_INPUT(HID_DATA | HID_ARRAY | HID_ABSOLUTE),

  HID_COLLECTION_END,

  // - Report 3: software communication -
  HID_USAGE_PAGE(HID_USAGE_PAGE_VENDOR),
  HID_USAGE(0x01),
  HID_COLLECTION(HID_COLLECTION_APPLICATION),
    HID_REPORT_ID(3)

    // TODO: definisci formato

  HID_COLLECTION_END
};

// TODO: poi in rust:
// device.write(&[3u8, /* dati config */])?;  // byte 0 = report ID

// // Read response
// let mut buf = [0u8; 65];
// device.read(&mut buf)?;


// USB HID
Adafruit_USBD_HID usbHid(desc_hid_report,         // descriptor HID
                         sizeof(desc_hid_report), // dimensione descriptor
                         HID_ITF_PROTOCOL_NONE,   // protocollo (mouse/tastiera/none) // TODO: cambia
                         POLLING_INTERVAL,         // polling interval (ms)
                         false                    // usa endpoint OUT
                       );


// --- Esempi di utilizzo ---

// // Tasto singolo (es. F5 = 0x3E)
// keyboardReport_t kbdReport = {0};
// kbdReport.keycodes[0] = 0x3E;
// usbHid.sendReport(1, &kbdReport, sizeof(kbdReport));
// // rilascio:
// memset(&kbdReport, 0, sizeof(kbdReport));
// usbHid.sendReport(1, &kbdReport, sizeof(kbdReport));

// // Combinazione Ctrl+Z
// kbdReport.modifier = 0x01; // LCtrl
// kbdReport.keycodes[0] = 0x1D; // Z
// usbHid.sendReport(1, &kbdReport, sizeof(kbdReport));

// // Volume su (encoder in senso orario)
// consumerReport_t cnsReport = { 0x00E9 }; // Volume Increment
// usbHid.sendReport(2, &cnsReport, sizeof(cnsReport));
// // rilascio obbligatorio per Consumer:
// cnsReport.usage = 0x0000;
// usbHid.sendReport(2, &cnsReport, sizeof(cnsReport));