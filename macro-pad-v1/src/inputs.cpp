#include <Arduino.h>
#include "headers/inputs.hpp"
#include "headers/hid.hpp"
#include "headers/config.hpp"


// ----- VARIABLES -----

MatrixBtn buttons[2] = {
  {0, 0},
  {1, 1}
};

uint8_t currentKeycodesIndex = 0; // The index at which to write the key in the report's keycodes array

// ----- FUNCTIONS -----

// - Main Inputs -

void readInputs() {
  uint8_t keyPressed;

   currentKeycodesIndex = 0;

  // TODO: pressione di più tasti insieme (bisogna attendere un attimo per vedere se sono premuti altri tasti?)?

  // if (!digitalRead(btn1)) {
    // keyPressed = keymap[0]; // TODO: fai mappatura tasti - keymap
    // setKeycode(keyPressed);
  // }

  for (const MatrixBtn &button : buttons) {
    if (!digitalRead(button.pin)) { // TODO: fai debounce e controllo press, double press, hold, release (uas oneButton?)
      keyboardReport.keycodes[currentKeycodesIndex++] = currentConfig.profiles[currentConfig.activeProfile].subprofiles[currentConfig.activeSubprofile].buttons[button.keymapIdx].onPress.value;
    }
  }

  readEncoder();
}


// - Utils -

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
