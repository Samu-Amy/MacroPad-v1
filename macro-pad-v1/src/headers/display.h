#pragma once
#include <Arduino.h>
#include <U8g2lib.h>


/*
  Display management
*/


// - Global Variables -

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


// - Function declarations -

void updateDisplay(U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2);
