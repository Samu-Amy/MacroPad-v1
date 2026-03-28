#pragma once
#include <Arduino.h>


/*
  The data of the device, used in config.h
*/


// ----- DEFINITIONS -----

// TODO: rendere personalizzabili questi valori (magari definendo un valore massimo (es. CONFIG_SIZE -> 4KB) per lo spazio utilizzabile e facendo un calcolo dello spazio occupato da questi valori, dando la possibilità di "distribuire" lo spazio che occupano in profili, sottoprofili, dimensione pool e altro)
#define PROFILE_COUNT 4
#define SUBPROFILE_COUNT 4
#define BUTTON_COUNT 6


// ----- TYPES -----

// Enums
enum class ActionType: uint8_t {
  NONE = 0,
  KEY,
  COMBO, // modifier + key
  MACRO, // keys (and modifier) sequence
  CONSUMER,
  PROFILE_SWITCH,
  SUBPROFILE_SWITCH
};


// TODO: si possono fare layer momentanei facendo lo swicth di profile/subprofile a hold/release (salvando in memoria il vecchio profile/subprofile)

// Structs
#pragma pack(push, 1) // Pack all structs (1 -> set max alignment to 1) - if not packed, the crc32 function could return different data because of "garbage" (random data) in empty bytes (padding), so in loadConfig would overwrite with default config

struct Action {                               // Tot: 4 Bytes, (alignment = 2)
  ActionType action;                          // 1 Byte (offset = 0,  alignment = 1)
  uint8_t modifier;                           // 1 Byte (o = 1,       a = 1)
  uint16_t value;                             // 2 Byte (o = 2-3,     a = 2)
};

struct ButtonBinding {                        // Tot: 16 Byte (alignment = 4)
  Action onPress;                             // 4 Byte (o = 0-3,   a = 2)
  Action onDouble;                            // 4 Byte (o = 4-7,   a = 2)
  Action onHold;                              // 4 Byte (o = 8-11,  a = 2)
  Action onRelease;                           // 4 Byte (o = 12,    a = 2)
};

struct Subprofile {                           // Tot: 124 Byte (alignment = 4)
  char name[16];                              // 16 Byte (o = 0, a = 1)
  Action encoderCW;                           // 4 Byte (o = 16, a = 2)
  Action encoderCCW;                          // 4 Byte (o = 20, a = 2)
  Action encoderPress;                        // 4 Byte (o = 24, a = 2)
  ButtonBinding buttons[BUTTON_COUNT];        // 16 * 6 -> 96 Byte (o = 28, a = 4)
};

struct Profile {                              // Tot: 513 (+ 3 Byte padding (if no packing) -> 516 Byte) (alignment = 4)
  char name[16];                              // 16 Byte (o = 0, a = 1)
  uint8_t subprofileCount;                    // 1 Byte (o = 16, a = 1)
  Subprofile subprofiles[SUBPROFILE_COUNT];   // 124 * 4 = 496 Byte (o = 17 -> 17 + 3 Byte padding (if no packing) -> o = 20, a = 4)
};

#pragma pack(pop)


// ----- FUNCTIONS -----

void initDevice();
void enableNumLock();
