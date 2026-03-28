#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "headers/device.hpp"
#include "headers/hid.hpp"


// ----- FUNCTIONS -----

// - Init -
// Do the actions needed when the the device is connected
void initDevice() {
  while (!usbHid.ready()) {
    tud_task(); // gestisce TinyUSB
    delay(1);
  }

  enableNumLock(); // Useful for having numpad keycodes work
}


// - Utils -

void enableNumLock() {
  keyboardReport.keycodes[0] = HID_KEY_NUM_LOCK;
  keyboardReport.modifier = 0;
  usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
  delay(10);

  // Release key
  keyboardReport.keycodes[0] = 0;
  usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
}
