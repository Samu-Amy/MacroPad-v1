#pragma once
#include <Arduino.h>

#include "device.h"


/*
  The data of the device (from device.h) saved in memory (device data, keymapping, ecc.), can be modified via the software and persist between firmware updates
*/


// - Definitions -

#define FLASH_SIZE_BYTES (2 * 1024 * 1024) // 2 MB (flash memory size)
#define CONFIG_SIZE 4096 // 4 KB
#define CONFIG_OFFSET (FLASH_SIZE_BYTES - CONFIG_SIZE) // Last page (relative to the start of the flash memory)

#define CONFIG_VERSION 1 // When changing the config structure -> change this value


// - Types -

struct Config {
  uint32_t crc;
  uint8_t version;
  char deviceName[32]; // ?
  uint8_t activeProfile;
  uint8_t activeSubprofile;
  Profile profiles[PROFILE_COUNT];
};


// - Global Variables -

extern Config currentConfig;


// - Function declarations -

uint32_t crc32(const void* data, size_t length);
uint32_t configCrc(const Config* cfg);
Config loadConfig();
void saveConfig(const Config* cfg);
