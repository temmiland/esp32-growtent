#include "config.h"

extern MatterDimmableLight DimmableLight;
extern void handleButtonDecommissioning();

void initializeSerialDebug() {
  // 115200 baud: standard rate for ESP32 USB communication
  Serial.begin(115200);
}

void initializeHardware() {
  // Configure PWM for LED (brightness control) and Fan (speed control)
  ledcAttach(ledPin, 250, 8);   // LED: 250 Hz, 8-bit resolution (0-255)
  ledcAttach(fanPin, 3000, 8);   // Fan: 3000 Hz, 8-bit resolution (0-255)
}

void waitForDecommissioningButtonPress() {
  // Button pressed: record timestamp
  if (digitalRead(buttonPin) == LOW && !button_state) {
    button_time_stamp = millis();
    button_state = true;
  }

  // Long press: decommission (5 seconds)
  if (button_state && (millis() - button_time_stamp) > decommissioningTimeout) {
    handleButtonDecommissioning();
  }
}
