#include "config.h"

extern MatterDimmableLight DimmableLight;
extern Preferences matterPref;
extern const char *lightOnOffPrefKey;
extern const char *brightnessPrefKey;


void initializeLightEndpoint() {
  // Restore last known state from flash memory
  // default to OFF and brightness 0 if no stored values found (first boot)
  bool lastOnOffState = matterPref.getBool(lightOnOffPrefKey, false);
  uint8_t lastBrightness = matterPref.getUChar(brightnessPrefKey, 0);

  setLightState(lastOnOffState, lastBrightness);
  DimmableLight.begin(lastOnOffState, lastBrightness);
  DimmableLight.onChange(setLightState);

  // Register callbacks for state changes
  DimmableLight.onChangeOnOff(handleLightOnOffChange);
  DimmableLight.onChangeBrightness(handleLightBrightnessChange);
}

void printLightCommissioningState() {
  Serial.printf("Initial light state: %s | brightness: %d\r\n", DimmableLight ? "ON" : "OFF", DimmableLight.getBrightness());
}

void updateLightAccessory() {
  DimmableLight.updateAccessory();
}

void decommissionLightAccessory() {
  Serial.println("Decommissioning the Light Matter Accessory. It shall be commissioned again.");
  DimmableLight = false;  // Turn off light
}

// Light callbacks

// PWM correction table (scaled to 70% of max range)
const uint8_t pwmPercent[] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
const uint8_t pwmValues[] = {179, 169, 155, 118, 94, 66, 48, 25, 7, 2, 0};

uint8_t interpolatePWM(uint8_t percent) {
  if (percent <= 0) return pwmValues[0];
  if (percent >= 100) return pwmValues[10];

  for (int i = 0; i < 10; i++) {
    if (pwmPercent[i] <= percent && percent <= pwmPercent[i + 1]) {
      uint8_t p1 = pwmPercent[i];
      uint8_t p2 = pwmPercent[i + 1];
      uint8_t v1 = pwmValues[i];
      uint8_t v2 = pwmValues[i + 1];

      float ratio = (float)(percent - p1) / (p2 - p1);
      return (uint8_t)(v1 + ratio * (v2 - v1));
    }
  }
  return pwmValues[10];
}

bool setLightState(bool state, uint8_t brightness) {
  uint8_t brightnessPercent = map(brightness, 0, 255, 0, 100);
  Serial.printf("Light Brightness Percent changed to %d\r\n", brightnessPercent);

  if (state) {
    uint8_t pwmValue = interpolatePWM(brightnessPercent);
    Serial.printf("Setting light brightness to %d%% (PWM: %d)\r\n", brightnessPercent, pwmValue);
    ledcWrite(ledPin, pwmValue);
  } else {
    ledcWrite(ledPin, 255);
  }
  matterPref.putUChar(brightnessPrefKey, brightness);
  matterPref.putBool(lightOnOffPrefKey, state);
  return true;
}

bool handleLightOnOffChange(bool state) {
  Serial.printf("Light OnOff changed to %s\r\n", state ? "ON" : "OFF");
  return true;
}

bool handleLightBrightnessChange(uint8_t level) {
  Serial.printf("Light Brightness changed to %d\r\n", level);
  return true;
}