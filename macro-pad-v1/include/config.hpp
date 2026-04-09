#pragma once
#include <Arduino.h>

#include "device.hpp"


/*
  The data of the device (from device.h) saved in memory (device data, keymapping, ecc.), can be modified via the software and persist between firmware updates
*/


// ----- DEFINITIONS -----

constexpr uint16_t FLASH_SIZE_BYTES = (2 * 1024 * 1024); // 2 MB (flash memory size)
constexpr uint16_t CONFIG_SIZE = 4096; // 4 KB (multiple of 256 bytes)
constexpr uint16_t CONFIG_OFFSET = (FLASH_SIZE_BYTES - CONFIG_SIZE); // Last page (relative to the start of the flash memory)

constexpr uint8_t CONFIG_VERSION = 1; // When changing the config structure (even PROFILE/SUBPROFILE/BUTTON _COUNT) -> change this value


// ----- TYPES -----

#pragma pack(push, 1) // Pack (avoid padding)
struct Config {
  uint32_t crc;
  uint8_t version;
  Settings settings;
  char deviceName[32];
  uint8_t activeProfile;
  uint8_t activeSubprofile;
  Profile profiles[PROFILE_COUNT];

  inline Profile& getActiveProfile() { return profiles[activeProfile % PROFILE_COUNT]; } // Prevent overflow with % check
  inline Subprofile& getActiveSubprofile() { return getActiveProfile().subprofiles[activeSubprofile % SUBPROFILE_COUNT]; }
};
#pragma pack(pop)


// ----- VARIABLES -----

extern Config currentConfig;


// ----- FUNCTION DECLARATIONS -----

Config loadConfig();
void saveConfig(const Config* config);
