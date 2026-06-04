#include <Arduino.h>

#include "inputs.hpp"
#include "hid.hpp"
#include "config.hpp"


// ----- VARIABLES -----

uint8_t currentKeycodesIndex = 0; // The index at which to write the key in the report's keycodes array


// ----- INTERNAL FUNCTION DECLARATIONS -----

constexpr bool isNumpadKey(uint16_t keycode);
static const ButtonBinding& getButtonAtIndex(uint8_t keymapIdx);


// ----- FUNCTIONS -----

// - Main Inputs -

void readInputs()
{
  // Reset the index at every iteration (since the report has been reset too)
  currentKeycodesIndex = 0;

  // TODO: pressione di più tasti insieme (bisogna attendere un attimo per vedere se sono premuti altri tasti?)?

  for (const MatrixBtn& button : buttons)
  {
    // TODO: fai debounce e controllo press, double press, hold, release (usa oneButton?)
    if (!digitalRead(button.pin) && currentKeycodesIndex < BUTTON_COUNT) // Using INPUT_PULLUP the read is inverted
    {
      const ButtonBinding& buttonBinding = getButtonAtIndex(button.keymapIdx);

      // Check for num lock (if setting is active)
      if (currentConfig.settings.has(SettingsFlag::ENSURE_NUM_LOCK) && isNumpadKey(buttonBinding.onPress.value)) {
        ensureNumLock();
      }

      // Set the value in the report and increment the index
      keyboardReport.keycodes[currentKeycodesIndex++] = buttonBinding.onPress.value;
    } // TODO: migliora (in base ai dati settati in config (non per forza solo keycodes))
  }

  if (!digitalRead(eSwitch))
  {
    currentConfig.settings.toggle(SettingsFlag::ENSURE_NUM_LOCK);
    }

    EncoderMode encoderMode = readEncoder(); // TODO: potrebbero non servire interrupts (servono se loop più lento o si gira velocemente e non si vuole perdere nessun "giro", forse servono in speed editor, fai setting (così si possono attivare/disattivare come il check del num lock)) - non gestire logica dentro interrupt, ma solo rilevamento del cambiamento (giro)
    if (encoderMode == EncoderMode::CW) {

    }
}


// - Utils -

// void setKeycode(uint8_t keycode) { // TODO: fare metodo al report per settare i dati (e spostare quindi anche "currentKeycodesIndex")?
  // keyboardReport.keycodes[keyIndex++] = keycode; // Set the first "empty" value to the pressed key
// }

// void debouce

EncoderMode readEncoder() // TODO: migliora (rilevamento di ogni cambiamento, non solo clk HIGI -> LOW ed aggiungi debouce)
{
  static int lastClk = HIGH;
  EncoderMode encoderVal = EncoderMode::NONE;

  int clk = digitalRead(eClk);
  int dt  = digitalRead(eDt);

  if (clk != lastClk && clk == LOW)
  {
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


// ----- INTERNAL FUNCTIONS -----

constexpr bool isNumpadKey(uint16_t keycode)
{
  if (keycode >= HID_KEY_KEYPAD_DIVIDE && keycode <= HID_KEY_KEYPAD_DECIMAL) return true;

  if (keycode == HID_KEY_KEYPAD_EQUAL) return true;
  
  if (keycode >= HID_KEY_KEYPAD_00 && keycode <= HID_KEY_KEYPAD_HEXADECIMAL) return true;

  return false;
}

static const ButtonBinding& getButtonAtIndex(uint8_t keymapIdx)
{
  return currentConfig.getActiveSubprofile().buttons[keymapIdx];
}
