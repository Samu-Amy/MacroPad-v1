#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#include "generics.hpp"
#include "device.hpp"
#include "hid.hpp"
#include "config.hpp"
#include "inputs.hpp"
#include "display.hpp"


// (forse no): fare firmware con solo parti che servono in base a cosa si usa (magari nel software avere dei flag per le varie cose (es. keys, consumer, ecc.) da usare per poter "ottimizzare" il firmware (eventualmente facendo un flash del firmware settando dei "#define" iniziali, usati poi con #if prima di compilarlo (ma serve compilazione)))
// TODO: aggiungere feature flags e impostazioni (tipo numero profili/sottoprofili, ecc. -> però se cambiano (es. un profilo in più) vanno "aggiornati manualmente" altrimenti il crc risulta diverso e vengono sovrascritti con quelli di default) del config (personalizzabili) per ottimizzare un po' (es. se non ci sono controllo consumer, non inviare mai il report (forse già così com'è può andare bene))


// TODO: trova modo di settare nome dispositivo e produttore

// - Setup -

void setup()
{
  // Init TinyUSB
  USBDevice.begin(0);
  usbHid.begin();

  // Set hid callbacks
  usbHid.setReportCallback(nullptr, onHidSetReport);

  // Init device
  initDevice();
  
  // Set pin modes
  for (const MatrixBtn &button : buttons) {
    pinMode(button.pin, INPUT_PULLUP);
  }

  pinMode(eClk, INPUT_PULLUP);
  pinMode(eDt, INPUT_PULLUP);
  pinMode(eSwitch, INPUT_PULLUP);
  
  // Init display
  u8g2.begin();
  initScreen();

  // Load config
  currentConfig = loadConfig();

  // Update data for display
  updateAllDisplayBuffers();

  // TODO: invia report con dati dispositivo per la connessione con il software (ed eventualmente finché non è collegato, invia il report ogni tot tempo ed ascolta per report di "richiesta connession" inviati dal software)
}


// - Loop -

void loop()
{
  // --- INIT ---

  tud_task();

  // Reset reports
  memset(&keyboardReport, 0, sizeof(keyboardReport));
  memset(&controlsReport, 0, sizeof(controlsReport));


  // --- INPUT ---
  
  // TODO: usare OneButton per input (?)

  readInputs();

  // --- SEND REPORTS ---

  static uint32_t lastReportUpdate = 0;
  const uint32_t nowReport = millis();

  if (nowReport - lastReportUpdate >= POLLING_INTERVAL)
  {
    static keyboardReport_t lastKeyboardReport;
    static consumerReport_t lastControlsReport;

    lastReportUpdate = nowReport;

    // TODO: risolvere caso in cui non si riesca ad inviare report "vuoto" (di rilascio tasti) dopo aver inviato quello con i dati (quindi nel caso si invii il report con i dati e dopo il polling interval si abbiano altri dati da mandare, mandando quindi quelli invece del "reset" (report "vuoto" di rilascio tasti))

    // Send keyboard report only if changed
    if (memcmp(&keyboardReport, &lastKeyboardReport, sizeof(keyboardReport)) != 0) // Compare reports
    {
      usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
      lastKeyboardReport = keyboardReport;
    }
    
    // Send consumer report only if changed
    if (memcmp(&controlsReport, &lastControlsReport, sizeof(controlsReport)) != 0) // Compare reports
    {
      usbHid.sendReport(2, &controlsReport, sizeof(controlsReport));
      lastControlsReport = controlsReport;
    }
  }


  // --- READ DATA ---
  
  // TODO: leggere dati ricevuti per aggiornare il display?


  // --- DISPLAY ---

  static uint32_t lastDisplayUpdate = 0; // TODO: fare funzione per applicare millis ad una funzione o blocco di codice (?)
  const uint32_t nowDisplay = millis();
  
  if (nowDisplay - lastDisplayUpdate > DISPLAY_UPDATE_TIMEOUT)
  {
    // Render the image
    updateDisplay();
    lastDisplayUpdate = nowDisplay;
  }
  
}
