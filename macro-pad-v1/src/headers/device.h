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


// Structs
struct Action {
  // Profile -> Subprofile -> keymap -> key action (single/macro) / device action (es. layer switch)
  ActionType action; // TODO: usa solo i bit necessari (se bastano 2 bit, usane 2 (con bitfields))?
  uint8_t modifier;
  uint16_t value;
  // uint8_t actionIndex; // The index of the action inside actionPool
};

// TODO: si possono fare layer momentanei facendo lo swicth di profile/subprofile a hold/release (salvando in memoria il vecchio profile/subprofile)
struct ButtonBinding {
  Action onPress;
  Action onDouble;
  Action onHold;
  Action onRelease;
};

struct Subprofile {
  char name[16];
  Action encoderCW;
  Action encoderCCW;
  Action encoderPress;
  ButtonBinding buttons[BUTTON_COUNT];
};

struct Profile {
  char name[16];
  uint8_t subprofileCount;
  Subprofile subprofiles[SUBPROFILE_COUNT];
};
