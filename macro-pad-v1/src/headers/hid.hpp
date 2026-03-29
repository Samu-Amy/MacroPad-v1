#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#include "headers/device.hpp"


/*
  The data of the device, used in config.h
*/


// ----- DEFINITIONS -----

#define POLLING_INTERVAL 10 //ms


// ----- TYPES -----

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
  // TODO: usa formato per comunicazione (id 3) - magari si possono anche fare "macro" tipo apri software o altro comunicando con il software
} communicationReport_t;


// ----- VARIABLES -----

extern Adafruit_USBD_HID usbHid;
extern keyboardReport_t keyboardReport;
extern consumerReport_t controlsReport;
extern communicationReport_t communicationReport;
