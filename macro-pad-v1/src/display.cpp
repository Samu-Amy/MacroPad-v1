#include <Arduino.h>
#include "headers/display.h"

// TODO: crea logo ed altre cose usando testi, linee ed altre forme, poi animali modificando i valori (tra cui la posizione)

// - Variables -

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



// - Functions -

void updateDisplay() { // TODO: aggiungi struct con dati da mostrare (oppure puntatore allo stato corrente (profilo e sottoprofilo selezionati, ecc.))
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr); // TODO: cambia font

  u8g2.drawStr(4, 8, "Blender - Viewport");

  // snprintf(buf, sizeof(buf), "Clk: %d  Dt: %d", clk, dt);
  // u8g2.drawStr(4, 20, buf);

  u8g2.sendBuffer();
  // TODO: usare millis per non scrivere sul display ad ogni iterazione?
}
