#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>
#include "headers/device.hpp"
#include "headers/hid.hpp"
#include "headers/config.hpp"
#include "headers/inputs.hpp"
#include "headers/display.hpp"


// (forse no): fare firmware con solo parti che servono in base a cosa si usa (magari nel software avere dei flag per le varie cose (es. keys, consumer, ecc.) da usare per poter "ottimizzare" il firmware (eventualmente facendo un flash del firmware settando dei "#define" iniziali, usati poi con #if prima di compilarlo (ma serve compilazione)))
// TODO: aggiungere feature flags e impostaizioni (tipo numero profili/sottoprofili, ecc. -> però se cambiano (es. un profilo in più) vanno "aggiornati manualmente" altrimenti il crc risulta diverso e vengono sovrascritti con quelli di default) del config (personalizzabili) per ottimizzare un po' (es. se non ci sono controllo consumer, non inviare mai il report (forse già così com'è può andare bene))


// - Setup -


void setup() {
  // Init TinyUSB
  USBDevice.begin(0);
  usbHid.begin();

  // Init device
  initDevice();
  
  // Set pin modes
  for (const MatrixBtn &button : buttons) {
    pinMode(button.pin, INPUT_PULLUP);
  }
  
  // Init display
  u8g2.begin();
  showLoadingScreen();

  // Load config
  currentConfig = loadConfig();
}


// - Loop -

void loop() {

  // --- INIT ---

  tud_task();

  // Reset reports
  memset(&keyboardReport, 0, sizeof(keyboardReport));
  memset(&controlsReport, 0, sizeof(controlsReport));


  // --- INPUT ---
  
  // TODO: usare OneButton per input (?)

  readInputs();

  // --- SEND REPORT ---

  static uint32_t lastReportUpdate = 0;
  if (millis() - lastReportUpdate >= POLLING_INTERVAL) {
    static keyboardReport_t last_keyboard_report;
    static consumerReport_t last_controls_report;

    lastReportUpdate = millis();

    // Send keyboard report only if changed
    if (memcmp(&keyboardReport, &last_keyboard_report, sizeof(keyboardReport)) != 0) { // Compare reports
      usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
      last_keyboard_report = keyboardReport;
    }
    
    // Send consumer report only if changed
    if (memcmp(&controlsReport, &last_controls_report, sizeof(controlsReport)) != 0) { // Compare reports
      usbHid.sendReport(2, &controlsReport, sizeof(controlsReport));
      last_controls_report = controlsReport;
    }
  }


  // --- READ DATA ---
  
  // TODO: leggere dati ricevuti per aggiornare il display?


  // --- DISPLAY ---

  static uint32_t lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate > 50) {
    updateDisplay();
    lastDisplayUpdate = millis();
  }
  
}
