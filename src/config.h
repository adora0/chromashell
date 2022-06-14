/*
 * CMake configured header
 */

#ifndef CONFIG_H
#define CONFIG_H

#define PROJECT_NAME            "chromashell"
#define VERSION_MAJOR           "1"
#define VERSION_MINOR           "0"
#define VERSION_YEAR            "2022"
#define VERSION_STRING          VERSION_MAJOR"."VERSION_MINOR
#define INSTALL_PREFIX          "/usr/local"
#define SYSTEM_DIR              "share/chromashell"
#define DEFAULT_PRESET_PATH     INSTALL_PREFIX"/"SYSTEM_DIR"/presets.json"

#endif
