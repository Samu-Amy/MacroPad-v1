#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>


/*
  The data of the device, used in config.h
*/


// - Definitions -

#define POLLING_INTERVAL 10 //ms


// - Types -

// Reports
typedef struct TU_ATTR_PACKED {
  uint8_t modifier; // bitmask
  uint8_t reserved; // always 0
  uint8_t keycodes[BUTTON_COUNT]; // Keycodes (0x00 -> key not pressed)
} keyboardReport_t;

typedef struct TU_ATTR_PACKED {
  uint16_t usage; // Consumer usage (0x0000 -> no keys)
} consumerReport_t;

typedef struct TU_ATTR_PACKED {
  // TODO: usa formato per comunicazione (id 3)
} communicationReport_t;


// - Variables -

extern Adafruit_USBD_HID usbHid;
