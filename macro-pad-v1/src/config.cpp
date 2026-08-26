#include "hardware/flash.h"
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>

#include "config.hpp"
#include "device.hpp"

// ----- ASSERTS -----

static_assert(sizeof(Config) <= CONFIG_SIZE, "Config too large!");                                                 // Check config size
static_assert(CONFIG_OFFSET % FLASH_SECTOR_SIZE == 0, "CONFIG_OFFSET must be sector-aligned (multiple of 4096)!"); // Check config offset

// ----- INTERNAL FUNCTION DECLARATIONS -----

static uint32_t crc32(const void* data, size_t length);
static uint32_t calculateConfigCrc(const Config* config);

// ----- VARIABLES -----

Config currentConfig = {};

// Pointer to the config in flash (read only via XIP)
static const Config* flashConfig = (const Config*)(XIP_BASE + CONFIG_OFFSET);

// Default config
static const Config defaultConfig = {
    .crc = 0,
    .version = CONFIG_VERSION,
    .settings = {
        .flags = 0 //? Per settare valori usa bitwise OR '|' and toMask() member function: "Settings::toMask(SettingsFlag::ENSURE_NUM_LOCK) | Settings::toMask(SettingsFlag::DISPLAY_PROFILE_INVERTED)"
    },
    .deviceName = "[Macro]Pad v1",
    .activeProfile = 0,
    .activeSubprofile = 0,
    .profiles = {{.name = "Blender", .subprofileCount = 1, .subprofiles = {{.name = "Viewport", .encoderCW = {ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_VOLUME_INCREMENT}, .encoderCCW = {ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_VOLUME_DECREMENT}, .encoderPress = {ActionType::CONSUMER, 0, HID_USAGE_CONSUMER_MUTE}, .buttons = {{.onPress = {ActionType::KEY, 0, HID_KEY_KEYPAD_1}}, {.onPress = {ActionType::KEY, 0, HID_KEY_KEYPAD_3}}, {.onPress = {ActionType::KEY, 0, HID_KEY_KEYPAD_7}}, {.onPress = {ActionType::KEY, 0, HID_KEY_1}}, {.onPress = {ActionType::KEY, 0, HID_KEY_2}}, {.onPress = {ActionType::KEY, 0, HID_KEY_3}}}}}}}};

// ----- FUNCTIONS -----

// - Memory -

void saveConfig(const Config* config)
{
  // TODO: aggiungi log per capire quando e quanto spesso viene chiamato
  // Can only write blocks of 256 bytes
  static uint8_t buf[CONFIG_SIZE]; // TODO: usare wear leveling (magari tramite librerie)?
  memset(buf, 0xFF, sizeof(buf));  // Reset buffer (set all bits to 1)
  memcpy(buf, config, sizeof(Config));

  // Disable interrupts while writing in flash memory (can't run code from flash while writing in it, so we need to disable interrupts because they could be called at any time)
  uint32_t ints = save_and_disable_interrupts();
  flash_range_erase(CONFIG_OFFSET, CONFIG_SIZE);
  flash_range_program(CONFIG_OFFSET, buf, CONFIG_SIZE);
  restore_interrupts(ints);
}

// Get and check the saved config, if something is wrong it overrides it with the default config
Config loadConfig()
{
  // Get the config from flash (copy it into a new Config (savedConfig))
  Config savedConfig;
  memcpy(&savedConfig, flashConfig, sizeof(Config));

  // Validate config (if different version or wrong crc -> load default)
  if (savedConfig.version != CONFIG_VERSION || savedConfig.crc != calculateConfigCrc(&savedConfig))
  {
    savedConfig = defaultConfig;                        // Overwrite config with default
    savedConfig.crc = calculateConfigCrc(&savedConfig); // Calculate the crc for the default config

    // Save the "new" default config in memory (if not equal)
    if (memcmp(flashConfig, &savedConfig, sizeof(Config)) != 0)
    {
      saveConfig(&savedConfig);
    }
  }

  return savedConfig;
}

// - Config -

static uint32_t calculateConfigCrc(const Config* config)
{
  // Skip the crc (start is a pointer to the memory address of the second field of config (version) (pointer to config (= start of the struct) + size of crc field (uint32_t))
  const uint8_t* start = (const uint8_t*)config + sizeof(uint32_t);

  // Calculate crc for config (without crc)
  return crc32(start, sizeof(Config) - sizeof(uint32_t));
}

// crc32 (Check data integrity)
static uint32_t crc32(const void* data, size_t length)
{
  const uint8_t* p = (const uint8_t*)data;
  uint32_t crc = 0xFFFFFFFF;

  for (size_t i = 0; i < length; i++)
  {
    crc ^= p[i]; // XOR with current byte (aggiungi il byte al "registro di divisione", come quando nella divisione si "portano giù" nuove cifre)

    for (uint8_t j = 0; j < 8; j++)
    { // for every bit of the current byte
      crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
      // (crc >> 1) -> Shift 1 bit to the right (lose LSB)

      // 0xEDB88320 -> crc32 polynomial reflected (because we use LSB instead of MSB)

      // (crc & 1) -> 1 is 0x00000001, with the & (AND), extract the LSB (the "last bit")
      // the '-' transform the value, 1 becomes -1 (since is unsigned, -1 gives the max value (0xFFFFFFFF)), 0 remains 0 (0x00000000)

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
