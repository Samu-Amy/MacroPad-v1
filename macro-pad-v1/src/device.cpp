#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#include "device.hpp"
#include "generics.hpp"
#include "hid.hpp"


// ----- VARIABLES -----

// volatile so it avoids caching from compiler's optimizations
volatile bool numLockActive = false;


// ----- FUNCTIONS -----

// - Init -

// Do the actions needed when the the device is connected
void initDevice()
{
  while (!usbHid.ready())
  {
    tud_task(); // gestisce TinyUSB
    delay(1);
  }
}


// - Callbacks -

// Callback used from hid to obtain informations from the pc
void onHidSetReport(uint8_t reportId, hid_report_type_t reportType, uint8_t const* buffer, uint16_t bufSize)
{
  if (reportId == 1 && reportType == HID_REPORT_TYPE_OUTPUT)
  {
    numLockActive = (buffer[0] & KEYBOARD_LED_NUMLOCK) != 0; // The num lock led status is at index 0 (buffer[0]), so we mask it and check
  }
}


// - Utils -

void ensureNumLock()
{
  // tud_task();

  if (numLockActive) return;

  // Press num lock key
  keyboardReport.keycodes[0] = HID_KEY_NUM_LOCK;
  keyboardReport.modifier = 0;
  usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
  delay(10);

  // Release key
  memset(&keyboardReport, 0, sizeof(keyboardReport));
  usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));

  // Wait the OS's response with the new status
  uint32_t start = millis();
  while (!numLockActive && (millis() - start < HID_RESPONSE_TIMEOUT))
  {
    tud_task();
    delay(1);
  }
}
