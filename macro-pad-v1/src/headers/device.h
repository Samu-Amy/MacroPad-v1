#pragma once
#include <Arduino.h>


/*
  The data of the device, used in config.h
*/


// - Definitions -

// TODO: rendere personalizzabili questi valori (magari definendo un valore massimo (es. CONFIG_SIZE -> 4KB) per lo spazio utilizzabile e facendo un calcolo dello spazio occupato da questi valori, dando la possibilità di "distribuire" lo spazio che occupano in profili, sottoprofili, dimensione pool e altro)
#define PROFILE_COUNT 4
#define SUBPROFILE_COUNT 4
#define BUTTON_COUNT 6


// - Types -

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
#pragma pack(push, 1) // Pack all structs

struct Action {                               // Tot: 4 Bytes (no padding -> packing not needed)
  ActionType action;                          // 1 Byte (offset = 0)
  uint8_t modifier;                           // 1 Byte (o = 1)
  uint16_t value;                             // 2 Byte (o = 2-3)
};

struct ButtonBinding {                        // Tot: 16 Byte (no padding -> packing not needed)
  Action onPress;                             // 4 Byte (o = 0-3)
  Action onDouble;                            // 4 Byte (o = 4-7)
  Action onHold;                              // 4 Byte (o = 8-11)
  Action onRelease;                           // 4 Byte (o = 12)
};

struct Subprofile {                           // Tot: 124 Byte
  char name[16];                              // 16 Byte (o = 0)
  Action encoderCW;                           // 4 Byte (o = 16)
  Action encoderCCW;                          // 4 Byte (o = 20)
  Action encoderPress;                        // 4 Byte (o = 24)
  ButtonBinding buttons[BUTTON_COUNT];        // 16 * 6 -> 96 Byte (o = 28)
};

struct Profile {                              // Tot: 513 (+ 3 Byte padding (if no packing) -> 516 Byte)
  char name[16];                              // 16 Byte (o = 0)
  uint8_t subprofileCount;                    // 1 Byte (o = 16)
  Subprofile subprofiles[SUBPROFILE_COUNT];   // 124 * 4 = 496 Byte (o = 17 -> 17 + 3 Byte padding (if no packing) -> o = 20)
};

#pragma pack(pop)


/*
Alignment, offset and padding (addedd in between variables/fields (not between element of an array, since they have all the same size/alignment, but can be added before the array))

Offset: where the field/data "should" starts (the field/data before ends) (es. in Subprofile -> name has offset 0 (start of struct), encoderCW has offset 16 (after the 16 Bytes of name), encoderCCW has offset 16 + 4 = 20)

Alignment: where the field/data can start (uint8_t -> 1 Byte, can start anywhere, uint16_t -> 2 Byte, can start at multiple of 2 (0, 2, 4, 6, 8, 10, ...), uint32_t -> 4 Byte, can start at multiple of 4 (0, 4, 8, 16, ...))
  for a struct is the max between all the alignment of the fields
  es.
    struct Example { // Size = 8 Byte, Alignment = 4 Byte (max(1, 2, 4))
      uint8_t a;  // size: 1 Byte, offset: 0, alignment: 1
      uint16_t b; // size: 2 Byte, offset: 1 + 1 (padding) = 2, alignment: 2
      uint32_t c; // size: 4 Byte, offset: 4, alignment: 4
    }

for every field:
  if offset % alignment == 0 -> ok (no padding) else padding

es. (in Profile):
  name -> offset = 0, alignment = 1 Byte (char) -> ok
  subprofileCount -> offset = 16, alignment = 1 Byte -> ok
  subprofiles -> offset = 17, alignment = 

*/
