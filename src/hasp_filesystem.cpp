/* MIT License - Copyright (c) 2019-2021 Francis Van Roie
   For full license information read the LICENSE file in the project folder */

#include "hasp_conf.h" // include first

#if HASP_USE_SPIFFS > 0 || HASP_USE_LITTLEFS > 0

#include <Arduino.h>
#include "ArduinoJson.h"
#include "ArduinoLog.h"

#include "hasp_debug.h"
#include "hasp_filesystem.h"

void filesystemInfo()
{ // Get all information of your SPIFFS
#if 0
    FSInfo fs_info;
    HASP_FS.info(fs_info);

    Serial.println("File system info.");

    Serial.print("Total space:      ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(fs_info.usedBytes);
    Serial.println("byte");

    Serial.print("Block size:       ");
    Serial.print(fs_info.blockSize);
    Serial.println("byte");

    Serial.print("Page size:        ");
    Serial.print(fs_info.totalBytes);
    Serial.println("byte");

    Serial.print("Max open files:   ");
    Serial.println(fs_info.maxOpenFiles);

    Serial.print("Max path lenght:  ");
    Serial.println(fs_info.maxPathLength);
    Serial.println("File sistem info.");

    Serial.print("Total space:      ");
    Serial.print(SPIFFS.totalBytes());
    Serial.println("byte");

    Serial.print("Total space used: ");
    Serial.print(SPIFFS.usedBytes());
    Serial.println("byte");

    Serial.print("Block size:       ");
    // Serial.print(SPIFFS);
    Serial.println("byte");

    Serial.print("Page size:        ");
    Serial.print(SPIFFS.totalBytes());
    Serial.println("byte");

    Serial.print("Max open files:   ");
    // Serial.println(SPIFFS.maxOpenFiles());

    Serial.print("Max path lenght:  ");
    // Serial.println(SPIFFS.maxPathLength());
#endif
}

void filesystemList()
{
#if HASP_USE_SPIFFS > 0
#if defined(ARDUINO_ARCH_ESP8266)
    if(!HASP_FS.begin()) {
#else
    if(!HASP_FS.begin(true)) { // default vfs path: /littlefs
#endif
        LOG_ERROR(TAG_FILE, F("Flash file system not mouted."));
    } else {

        LOG_VERBOSE(TAG_FILE, F("Listing files on the internal flash:"));

#if defined(ARDUINO_ARCH_ESP32)
        File root = HASP_FS.open("/");
        File file = root.openNextFile();
        while(file) {
            LOG_VERBOSE(TAG_FILE, F("   * %s  (%u bytes)"), file.name(), (uint32_t)file.size());
            file = root.openNextFile();
        }
#endif
#if defined(ARDUINO_ARCH_ESP8266)
        Dir dir = HASP_FS.openDir("/");
        while(dir.next()) {
            LOG_VERBOSE(TAG_FILE, F("   * %s  (%u bytes)"), dir.fileName().c_str(), (uint32_t)dir.fileSize());
        }
#endif
    }
#endif
}

bool filesystemSetup(void)
{
    // no SPIFFS settings, as settings depend on SPIFFS
    // no Logging, because it depends on the configuration file

    // Logging is defered until debugging has started
    // FS success or failure is printed at that time !

#if HASP_USE_SPIFFS > 0 || HASP_USE_LITTLEFS > 0
#if defined(ARDUINO_ARCH_ESP8266)
    if(!HASP_FS.begin()) {
#else
    if(!HASP_FS.begin(true)) {
#endif
        // LOG_ERROR(TAG_FILE, F("SPI flash init failed. Unable to mount FS."));
        return false;
    } else {
        // LOG_INFO(TAG_FILE, F("SPI Flash FS mounted"));
        return true;
    }
#endif

    return false;
}

#endif