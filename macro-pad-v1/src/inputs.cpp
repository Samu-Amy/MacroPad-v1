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
    if (!digitalRead(button.pin) && currentKeycodesIndex < BUTTON_COUNT) { // TODO: fai debounce e controllo press, double press, hold, release (uas oneButton?)
      keyboardReport.keycodes[currentKeycodesIndex++] = currentConfig.profiles[currentConfig.activeProfile].subprofiles[currentConfig.activeSubprofile].buttons[button.keymapIdx].onPress.value;
    } // TODO: fai una cosa migliore (in base ai dati settati in config (non per forza solo keycodes))
  }

  if (!digitalRead(eSwitch)) {
    currentConfig.settings.toggle(SettingsFlag::DISPLAY__PROFILE_INVERTED); // [TEST]
  }

  EncoderMode encoderMode = readEncoder(); // TODO: potrebbero non servire interrupts (servono se loop più lento o si gira velocemente e non si vuole perdere nessun "giro") - non gestire logica dentro interrupt, ma solo rilevamento del cambiamento (giro)
  if (encoderMode == EncoderMode::CW) {

  }
}


// - Utils -

// void setKeycode(uint8_t keycode) { // TODO: fare metodo al report per settare i dati (e spostare quindi anche "currentKeycodesIndex")?
  // keyboardReport.keycodes[keyIndex++] = keycode; // Set the first "empty" value to the pressed key
// }

// void debouce

EncoderMode readEncoder() { // TODO: migliora (rilevamento di ogni cambiamento, non solo clk HIGI -> LOW ed aggiungi debouce)
  static int lastClk = HIGH;
  EncoderMode encoderVal = EncoderMode::NONE;

  int clk = digitalRead(eClk);
  int dt  = digitalRead(eDt);

  if (clk != lastClk && clk == LOW) {
    if (dt != clk) {
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
