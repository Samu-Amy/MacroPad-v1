#pragma once
#include <Arduino.h>


/*
  Manages the inputs (buttons and encoder)
*/


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


// ----- CONSTANTS -----

// Encoder
constexpr pin_size_t eClk = 6;
constexpr pin_size_t eDt = 7;
constexpr pin_size_t eSwitch = 8;


// ----- VARIABLES -----

extern MatrixBtn buttons[2];


// ----- FUNCTIONS -----

void readInputs();
// void setKeycode(uint8_t keycode);
EncoderMode readEncoder();
