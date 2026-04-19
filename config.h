#ifndef CONFIG_H
#define CONFIG_H

#include <Preferences.h>

// WiFi Configuration
extern const char *ssid;
extern const char *password;

// Preferences & Storage Keys
extern Preferences matterPref;
extern const char *lightOnOffPrefKey;
extern const char *fanOnOffPrefKey;
extern const char *brightnessPrefKey;
extern const char *fanSpeedPrefKey;

// Hardware Pins
const uint8_t ledPin = 18;
const uint8_t fanPin = 19;
const uint8_t buttonPin = BOOT_PIN;

// Button Configuration
extern uint32_t button_time_stamp;
extern bool button_state;
extern const uint32_t decommissioningTimeout;

#endif // CONFIG_H
