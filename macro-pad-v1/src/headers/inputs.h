#pragma once
#include <Arduino.h>


/*
  Manages the inputs (buttons and encoder)
*/


// - Definitions -

// Buttons
#define btn1 1
#define btn2 0

// Encoder
#define eClk 6
#define eDt 7
#define eSw 8


// - Types -

enum class EncoderMode: uint8_t {
  CW = 0,
  CCW,
  NONE
};


// - Function declarations -
void readInputs();
void setKeycode(uint8_t keycode);
EncoderMode readEncoder();
