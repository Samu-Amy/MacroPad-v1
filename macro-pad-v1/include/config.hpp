#pragma once
#include <Arduino.h>

#include "device.hpp"


/*
  The data of the device (from device.h) saved in memory (device data, keymapping, ecc.), can be modified via the software and persist between firmware updates
*/


// ----- CONSTANTS -----

constexpr uint16_t CONFIG_SIZE = 4096; // 4 KB (must be multiple of 4KB) - can be changed (multiplying it with an integer)

constexpr uint32_t FLASH_SIZE_BYTES = (2 * 1024 * 1024); // 2 MB (flash memory size) - depends on the microcontroller
constexpr uint32_t CONFIG_OFFSET = (FLASH_SIZE_BYTES - CONFIG_SIZE); // Last page (relative to the start of the flash memory)

constexpr uint8_t CONFIG_VERSION = 2; // Change this value the config structure changes (even [PROFILE/SUBPROFILE/BUTTON]_COUNT)


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


// ----- FUNCTIONS -----

Config loadConfig();
void saveConfig(const Config* config);
