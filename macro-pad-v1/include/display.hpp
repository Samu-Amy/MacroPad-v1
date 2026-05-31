#pragma once
#include <Arduino.h>
#include <U8g2lib.h>
#include "device.hpp"


/*
  Display management
*/


// ----- VARIABLES -----

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;


// ----- FUNCTIONS -----

void initScreen();
void updateDisplay();

void updateAllDisplayBuffers();
void updateSubprofileDisplayBuffer();

void drawCentered(u8g2_uint_t y, const char* text);
