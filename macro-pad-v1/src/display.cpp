#include <Arduino.h>
#include "headers/display.hpp"

#include "headers/config.hpp"

// TODO: crea logo ed altre cose usando testi, linee ed altre forme, poi animali modificando i valori (tra cui la posizione)

// ----- VARIABLES -----

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

int ascent;
int descent; // negative number

char profileBuf[16];
char subprofileBuf[16];


// TODO: modalità display con icone/testi che mostrano cosa fanno i tasti (?)


// ----- FUNCTIONS -----

// - Main -

// Init
void initScreen() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tf);

  ascent = u8g2.getAscent();
  descent = u8g2.getDescent();

  // u8g2.drawStr(4, 8, "Loading config...");

  u8g2.sendBuffer();
}

// Update
void updateDisplay() { // TODO: aggiungi struct con dati da mostrare (oppure puntatore allo stato corrente (profilo e sottoprofilo selezionati, ecc.))
  u8g2.clearBuffer();
  
  // - Profile -
  u8g2.drawBox(0, 0, 128, 14);
  u8g2.setDrawColor(0); // Invert color
  drawCentered(12, profileBuf);
  
  u8g2.setDrawColor(1);
  
  // - Subprofile -
  u8g2.drawStr(4, 28, subprofileBuf);

  // - Data -

  u8g2.sendBuffer();
  // TODO: usare millis per non scrivere sul display ad ogni iterazione?
}

// - Utils -

void updateAllDisplayBuffers() {
  Profile& currentProfile = currentConfig.profiles[currentConfig.activeProfile];

  // Update profile buffer
  snprintf(profileBuf, sizeof(profileBuf), "%.15s", currentProfile.name); // %.15s so it only reads 15 chars (in case of missing '\0' as 16th char, it doesn't continue to read values)

  updateSubprofileDisplayBuffer(currentProfile);
}

void updateSubprofileDisplayBuffer(Profile& currentProfile) {

  // Update subprofile buffer
  snprintf(subprofileBuf, sizeof(subprofileBuf), "%.15s", currentProfile.subprofiles[currentConfig.activeSubprofile].name);

  // Update data
}

void drawCentered(u8g2_uint_t y, const char* text) {
  u8g2.drawStr(64 - u8g2.getUTF8Width(text) / 2, y + descent, text);
}
