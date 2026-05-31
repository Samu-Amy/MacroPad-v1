#pragma once
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#include "device.hpp"


/*
  The data of the device, used in config.h
*/


// ----- CONSTANTS -----

constexpr uint8_t POLLING_INTERVAL = 10; //ms

// Number of Keys that can be pressed at the same time
constexpr uint8_t BUTTON_COUNT = 6;


// ----- TYPES -----

// - Enums -

enum class Command: uint8_t { // TODO: potrebbero essere da modificare
  GET_CONFIG = 0x01,
  SET_CONFIG,
  GET_INFO,
  REBOOT_UF2,
  AKC,
  ERROR
};


// - Reports -

typedef struct TU_ATTR_PACKED {
  uint8_t modifier; // bitmask
  uint8_t reserved; // always 0
  uint8_t keycodes[BUTTON_COUNT]; // Keycodes (0x00 -> key not pressed)
} keyboardReport_t;

typedef struct TU_ATTR_PACKED {
  uint16_t usage; // Consumer usage (0x0000 -> no keys)
} consumerReport_t;

typedef struct TU_ATTR_PACKED { // TODO: magari si possono anche fare "macro" tipo apri software o altro comunicando con il software
  Command command;
  uint8_t packet_index;
  uint8_t packet_total;
  uint8_t data_length;
  uint8_t data[59];
} communicationReport_t;


// ----- VARIABLES -----

extern Adafruit_USBD_HID usbHid;
extern keyboardReport_t keyboardReport;
extern consumerReport_t controlsReport;
extern communicationReport_t communicationReport;
