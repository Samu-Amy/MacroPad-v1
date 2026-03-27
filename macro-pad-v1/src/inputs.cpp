#include <Arduino.h>
#include "headers/inputs.h"


// - Variables -

uint8_t currentKeycodesIndex = 0; // The index at which to write the key in the report's keycodes array


// - Main Inputs -

void readInputs() {
  uint8_t keyPressed;

   currentKeycodesIndex = 0;

  // TODO: pressione di più tasti insieme (bisogna attendere un attimo per vedere se sono premuti altri tasti?)?

  if (!digitalRead(btn1)) {
    // keyPressed = keymap[0]; // TODO: fai mappatura tasti - keymap
    setKeycode(keyPressed);
  }

  readEncoder();
}


// - Utils functions -

// void setKeycode(uint8_t keycode) { // TODO: fare metodo al report per settare i dati (e spostare quindi anche "currentKeycodesIndex")?
  // keyboardReport.keycodes[keyIndex++] = keycode; // Set the first "empty" value to the pressed key
// }

EncoderMode readEncoder() {
  static int lastClk = HIGH;
  EncoderMode encoderVal = EncoderMode::NONE;

  int clk = digitalRead(eClk);
  if (clk != lastClk && clk == LOW) {
    if (digitalRead(eDt) != clk) {
      encoderVal = EncoderMode::CW;
    } else {
      encoderVal = EncoderMode::CCW;
    }
  } else {
    encoderVal = EncoderMode::NONE;
  }

  lastClk = clk;
  return encoderVal;
}
