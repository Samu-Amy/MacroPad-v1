#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#include "headers/config.h"


// - Variables -

Config currentConfig;

// Pointer to config in flash (read only via XIP)
static const Config* flashConfig = (const Config*)(XIP_BASE + CONFIG_OFFSET);

// Default config
static const Config defaultConfig = {
  .crc = 0,
  .version = CONFIG_VERSION,
  .activeProfile = 0,
  .activeSubprofile = 0,
  .profiles = {{
    .name = "Default",
    .subprofileCount = 1,
    .subprofiles = {{
      .name = "Main",
      .encoderCW = { ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_VOLUME_INCREMENT },
      .encoderCCW = { ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_VOLUME_DECREMENT },
      .encoderPress = { ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_MUTE },
      .buttons = {
        { .onPress = { ActionType::KEY, 0, 0x04 } },
        { .onPress = { ActionType::KEY, 0, 0x3F } }
      }
    }}
  }}
}


// - Functions -

// Check data integrity
uint32_t crc32(const void* data, size_t length) {
  const uint8_t* p = (const uint8_t*)data;
  uint32_t crc = 0xFFFFFFFF;

  for (size_t i = 0; i < length; i++) {
    crc ^= p[i]; // XOR with current byte (aggiungi il byte al "registro di divisione", come quando nella divisione si "portano giù" nuove cifre)

    for (uint8_t j = 0; j < 8; j++) { // for every bit of the current byte
      crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
      // (crc >> 1) -> Shift 1 bit to the right (lose LSB)

      // 0xEDB88320 -> crc32 polynomial reflected (because we use LSB instead of MSB)

      // (crc & 1) -> 1 is 0x00000001, with the & (AND), extract the LSB (the "last bit")
      // the '-' transform the value, 1 becomes -1 (0xFFFFFFFF), 0 remains 0 (0x00000000)

      // the & (AND) after the polynomial is used as an if (if the LSB was 1, so now is -1 (0xFFFFFFFF) -> the XOR is "applied", since the polynomial pass the &, if the LSB was 0, the polynomial & 0 = 0 -> XOR with 0)
      // so if the bit was 1 -> "compensate" using the XOR with the polynomial
    }
  }

  return ~crc; // Invert bits
}

// crc32 fa una divisione polinomiale (con il polinomio, che è progettato per riconoscere errori (bit flip, burst error, ecc.))
// usando i dati come se fossero un unico numero binario e ritornando il crc (che è il resto della divisione),
// per ogni bit si controlla il resto parziale (se il valore è 1 bisogna correggere -> XOR (sottrazione in algebra binaria) con polinomio -> sottrai il divisore)
// ("se (parte corrente >= divisore) -> sottrai divisore")

// In pratica:
// - con "crc ^= p[i];" aggiungi il bit al registro, ma senza riporti
// quindi se il resto “richiede sottrazione”, sottrai (XOR con) il polinomio
