#include "config.h"

extern MatterFan Fan;
extern Preferences matterPref;
extern const char *fanOnOffPrefKey;
extern const char *fanSpeedPrefKey;

void initializeFanEndpoint() {
  // Restore last known state from flash memory
  // default to OFF and speed 0 if no stored values found (first boot)
  bool lastFanOnOffState = matterPref.getBool(fanOnOffPrefKey, false);
  uint8_t lastSpeed = matterPref.getUChar(fanSpeedPrefKey, 0);

  Fan.begin(lastSpeed, lastFanOnOffState ? MatterFan::FAN_MODE_LOW : MatterFan::FAN_MODE_OFF, MatterFan::FAN_MODE_SEQ_OFF_LOW_MED_HIGH);
  setFanState(lastFanOnOffState, lastSpeed);

  // Register callbacks for state changes
  Fan.onChangeSpeedPercent(handleFanSpeedChange);
  Fan.onChangeMode(handleFanModeChange);
  Fan.onChange(handleFanStateChange);
}

void printFanCommissioningState() {
  Serial.printf("Initial fan state: %s | speed: %d\r\n", Fan ? "ON" : "OFF", Fan.getSpeedPercent());
}

void updateFanAccessory() {
  Fan.updateAccessory();
}

void decommissionFanAccessory() {
  Serial.println("Decommissioning the Fan Matter Accessory. It shall be commissioned again.");
  Fan = false;  // Turn off fan
}

// Fan callbacks

bool setFanState(bool state, uint8_t speed) {
  if (state) {
    ledcWrite(fanPin, (255 - speed) * 0.7);
  } else {
    ledcWrite(fanPin, 255);
  }
  matterPref.putUChar(fanSpeedPrefKey, speed);
  matterPref.putBool(fanOnOffPrefKey, state);
  return true;
}

bool handleFanSpeedChange(uint8_t speedPercent) {
  // Keep speed and mode in sync: if speed is 0, turn fan OFF; if speed > 0, turn fan ON

  // Speed set to 0 while fan is ON → turn OFF
  if (speedPercent == MatterFan::OFF_SPEED && Fan.getMode() != MatterFan::FAN_MODE_OFF) {
    return Fan.setOnOff(false, Fan.ATTR_SET);
  }

  // Speed set to > 0 while fan is OFF → turn ON
  if (speedPercent > MatterFan::OFF_SPEED && Fan.getMode() == MatterFan::FAN_MODE_OFF) {
    return Fan.setOnOff(true, Fan.ATTR_SET);
  }

  // All other cases: speed change is valid, no mode change needed
  return true;
}

bool handleFanModeChange(MatterFan::FanMode_t fanMode) {
  if (fanMode != MatterFan::FAN_MODE_OFF) {
    uint8_t desiredSpeed = 0;
    Serial.printf("Fan set to %s mode -- speed percentage will go to %d%%\r\n", Fan.getFanModeString(fanMode), desiredSpeed);

    switch (fanMode) {
      case MatterFan::FAN_MODE_LOW:
        desiredSpeed = 35;
        break;
      case MatterFan::FAN_MODE_MEDIUM:
        desiredSpeed = 60;
        break;
      case MatterFan::FAN_MODE_HIGH:
        desiredSpeed = 100;
        break;
    }
    return Fan.setSpeedPercent(desiredSpeed, Fan.ATTR_SET);
  }
  return true;
}

bool handleFanStateChange(MatterFan::FanMode_t fanMode, uint8_t speedPercent) {
  Serial.printf("Fan State: Mode %s | %d%% speed.\r\n", Fan.getFanModeString(fanMode), speedPercent);
  setFanState(fanMode != MatterFan::FAN_MODE_OFF, speedPercent);
  return true;
}
