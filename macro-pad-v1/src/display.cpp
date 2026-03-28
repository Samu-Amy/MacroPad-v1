#include <Arduino.h>
#include "headers/display.hpp"

#include "headers/config.hpp"

// TODO: crea logo ed altre cose usando testi, linee ed altre forme, poi animali modificando i valori (tra cui la posizione)

// ----- VARIABLES -----

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

char buf[36]; // profileName (15 chars + '\0') + " - " (3 chars) + subprofileName (15 chars + '\0') + '\0'



// ----- FUNCTIONS -----

void updateDisplay() { // TODO: aggiungi struct con dati da mostrare (oppure puntatore allo stato corrente (profilo e sottoprofilo selezionati, ecc.))
  Profile& currentProfile = currentConfig.profiles[currentConfig.activeProfile];
  
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // TODO: cambia font


  snprintf(buf, sizeof(buf), "%.15s - %.15s", currentProfile.name, currentProfile.subprofiles[currentConfig.activeSubprofile].name); // %.15s so it only reads 15 chars (in case of missing '\0' as 16th char, it doesn't continue to read values)
  u8g2.drawStr(4, 8, buf);
  
  // [DEBUG]
  if (!usbReady) {
    u8g2.drawStr(48, 64, "USB not ready");
  } else {
    u8g2.drawStr(48, 64, "USB ready");
  }

  u8g2.sendBuffer();
  // TODO: usare millis per non scrivere sul display ad ogni iterazione?
}

void showLoadingScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // TODO: cambia font

  u8g2.drawStr(4, 8, "Loading config...");

  u8g2.sendBuffer();
}
