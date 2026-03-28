#include <Arduino.h>
#include "headers/display.hpp"

#include "headers/config.hpp"

// TODO: crea logo ed altre cose usando testi, linee ed altre forme, poi animali modificando i valori (tra cui la posizione)

// ----- VARIABLES -----

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

char profileBuf[16];
char subprofileBuf[16];



// ----- FUNCTIONS -----

void updateDisplay() { // TODO: aggiungi struct con dati da mostrare (oppure puntatore allo stato corrente (profilo e sottoprofilo selezionati, ecc.))
  Profile& currentProfile = currentConfig.profiles[currentConfig.activeProfile];
  
  u8g2.clearBuffer();
  // u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setFont(u8g2_font_6x10_tf);
  
  // TODO: non settare il buffer ad ogni frame, ma solo quando cambia (quindi non farlo qua)
  snprintf(profileBuf, sizeof(profileBuf), "%.15s", currentProfile.name); // %.15s so it only reads 15 chars (in case of missing '\0' as 16th char, it doesn't continue to read values)
  u8g2.drawStr(2, 8, profileBuf);
  
  snprintf(subprofileBuf, sizeof(subprofileBuf), "%.15s", currentProfile.subprofiles[currentConfig.activeSubprofile].name);
  u8g2.drawStr(10, 20, subprofileBuf);

  u8g2.sendBuffer();
  // TODO: usare millis per non scrivere sul display ad ogni iterazione?
}

void showLoadingScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  u8g2.drawStr(4, 8, "Loading config...");

  u8g2.sendBuffer();
}
