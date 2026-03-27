#include <Arduino.h>

#include <U8g2lib.h>
#include <Wire.h>
#include "headers/device.h"
#include "headers/hid.h"
#include "headers/config.h"
#include "headers/inputs.h"
#include "headers/display.h"

// Per salvare i config nell'ultima parte di memoria
// #include "pico/flash.h"
// #include "hardware/flash.h"


// (forse no): fare firmware con solo parti che servono in base a cosa si usa (magari nel software avere dei flag per le varie cose (es. keys, consumer, ecc.) da usare per poter "ottimizzare" il firmware (eventualmente facendo un flash del firmware settando dei "#define" iniziali, usati poi con #if prima di compilarlo (ma serve compilazione)))
// TODO: aggiungere feature flags e impostaizioni (tipo numero profili/sottoprofili, ecc. -> però se cambiano (es. un profilo in più) vanno "aggiornati manualmente" altrimenti il crc risulta diverso e vengono sovrascritti con quelli di default) del config (personalizzabili) per ottimizzare un po' (es. se non ci sono controllo consumer, non inviare mai il report (forse già così com'è può andare bene))


// - Setup -

void setup() {
  USBDevice.begin(0);
  
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  u8g2.begin();

  // Salva default config in flash se non c'è già una config salvata

  // TODO: usare wear leveling (magari tramite librerie)?

  // ----- Opzione nuova (checksum)
  // // Load config
  // macropadReport_t config;
  // load_config(&config, sizeof(config));
  // configChecksum = crc32(config)

  // if (crc32(config) != config.stored_crc) { // ATTENZIONE: non esiste crc32(), va creata (tipo sotto)
  //     // Config non valida → carica default
  //     config = default_config;
  //     config.stored_crc = crc32(config); // crc32(config) is a uint32_t
  //     save_config(&config, sizeof(config));
  // }

  // // Con la funzione crc32 (custom) sarebbe:
  // macropadReport_t config;
  // load_config(&config, sizeof(config));
  // if (crc32(((uint8_t*)&config)+4, sizeof(config)-4) != config.crc) {
  //   config = default_config;               // primo avvio o flash corrotta
  //   config.crc = crc32(((uint8_t*)&config)+4, sizeof(config)-4);
  //   save_config(&config, sizeof(config));
  // }
  // -----

  // ----- Opzione vecchia
  // macropadReport_t config;
  // load_config(&config, sizeof(config)); // ottiene dati config (se non ancora salvati, valori "casuali")

  // if (config.magic != EXPECTED_MAGIC) { // primo avvio (magic è un campo nella struct config per indicare che la struct è valida (dati salvati)) -> se il valore di magic è diverso da quello che dovrebbe avere (nessuna config salvata e quindi dati "casuali") -> nessuna config è salvata e quindi si può scrivere quella di default
  //     config = default_config;          // carica valori di default
  //     save_config(&config, sizeof(config));
  // }
  // -----
}


// - Loop -

keyboardReport_t keyboardReport;
consumerReport_t controlsReport;

// char buf[32];

void loop() {

  // --- INIT ---

  tud_task();

  // Wait for usb to be ready
  if (!usbHid.ready()) return;

  // Reset reports
  memset(&keyboardReport, 0, sizeof(keyboardReport));
  memset(&controlsReport, 0, sizeof(controlsReport));


  // --- INPUT ---
  
  // TODO: usare OneButton per input (?)

  readInputs();

  // --- SEND REPORT ---

  static uint32_t last = 0;
  if (millis() - last >= POLLING_INTERVAL) {
    static keyboardReport_t last_keyboard_report;
    static consumerReport_t last_controls_report;

    last = millis();

    // Send keyboard report only if changed
    if (memcmp(&keyboardReport, &last_keyboard_report, sizeof(keyboardReport)) != 0) { // Compare reports
      usbHid.sendReport(1, &keyboardReport, sizeof(keyboardReport));
      last_keyboard_report = keyboardReport;
    }
    
    // Send keyboard report only if changed
    if (memcmp(&controlsReport, &last_controls_report, sizeof(controlsReport)) != 0) { // Compare reports
      usbHid.sendReport(2, &controlsReport, sizeof(controlsReport));
      last_controls_report = controlsReport;
    }
  }


  // --- READ DATA ---
  
  // TODO: leggere dati ricevuti per aggiornare il display?


  // --- DISPLAY ---

  updateDisplay(u8g2);
  
}


// ----- FUNCTIONS -----

// TODO: sposta in file a parte (tipo memory, config o device)

void enterBootloader() {
  // reset_usb_boot(0, 0);
}

// void tud_hid_set_report_cb(...) {
//     // dati ricevuti dal PC -> si possono usare per aggiornare la keymap (usando enterBootloader e caricando il file .uf2) o richiederla (per backup/condivisione)
// }

// TODO: salva il meno possibile, solo quando necessario (numero di scritture limitato)
// void save_config(const void* config_data, size_t size) {
//   if (size <= CONFIG_SIZE) { // Check // TODO: se non la salva inviare un messaggio di errore (?)
//     flash_range_erase(CONFIG_OFFSET, CONFIG_SIZE);
//     flash_range_program(CONFIG_OFFSET, (const uint8_t*)config_data, size);
//   }
// }

// void load_config(void* config_data, size_t size) {
//     memcpy(config_data, (void*)(XIP_BASE + CONFIG_OFFSET), size);
// }

// uint32_t crc32(const void *data, size_t length) {
//     const uint8_t *p = data;
//     uint32_t crc = 0xFFFFFFFF;
//     for (size_t i = 0; i < length; i++) {
//         crc ^= p[i];
//         for (uint8_t j = 0; j < 8; j++) {
//             crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
//         }
//     }
//     return ~crc;
// }

// Default config: salvata nella RAM, se flash libera -> copia nella flash, altrimenti no (usa quella nella flash) -> per evitare overwrite delle config custom all'update del firmware

// flusso tipo qmk/via:
// - chiama tud_task()
// - legge input
// - invia HID keyboard
// - riceve config via callback
// - salva in flash
