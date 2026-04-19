#include <Matter.h>
#include <Preferences.h>

// Matter Endpoints
MatterDimmableLight DimmableLight;
MatterFan           Fan;

// WiFi Configuration (set via .env file at build time)
const char *ssid = "YourNetwork";
const char *password = "YourPassword";

// Preferences & Storage
Preferences matterPref;
const char *lightOnOffPrefKey = "LightOnOff";
const char *fanOnOffPrefKey = "FanOnOff";
const char *brightnessPrefKey = "Brightness";
const char *fanSpeedPrefKey = "FanSpeed";

// Button Control
uint32_t button_time_stamp = 0;
bool button_state = false;
const uint32_t decommissioningTimeout = 5000;
