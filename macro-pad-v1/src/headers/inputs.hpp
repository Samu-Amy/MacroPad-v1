#pragma once
#include <Arduino.h>


/*
  Manages the inputs (buttons and encoder)
*/


// ----- DEFINITIONS -----

// Encoder
#define eClk 6
#define eDt 7
#define eSwitch 8


// ----- TYPES -----

struct MatrixBtn {
  uint8_t pin;    // hardware pin
  uint8_t keymapIdx;  // index in the keymap
};

enum class EncoderMode: uint8_t {
  CW = 0,
  CCW,
  NONE
};


// ----- VARIABLES -----

extern MatrixBtn buttons[2];


// ----- FUNCTION DECLARATIONS -----

void readInputs();
// void setKeycode(uint8_t keycode);
EncoderMode readEncoder();
