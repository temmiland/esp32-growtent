#include <Matter.h>
#include <WiFi.h>
#include <Preferences.h>
#include "config.h"

extern MatterDimmableLight DimmableLight;
extern MatterFan Fan;

void setup() {
  initializeSerialDebug();
  initializeHardware();
  initializeWiFi();
  initializeMatter();
  checkMatterCommissioning(DimmableLight, Fan);
}

void loop() {
  handleSerialPWMTest();
  waitForMatterCommissioning(DimmableLight, Fan);
  waitForDecommissioningButtonPress();
}

void handleSerialPWMTest() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() == 0) return;

    if (input.equalsIgnoreCase("help")) {
      Serial.println("\n=== PWM Test Mode ===");
      Serial.println("Enter PWM value (0-255) to test");
      Serial.println("0 = 100% (95W) | 255 = OFF");
      Serial.println("Commands: help, off, status");
      return;
    }

    if (input.equalsIgnoreCase("off")) {
      ledcWrite(ledPin, 255);
      Serial.println("Light OFF (PWM 255)");
      return;
    }

    if (input.equalsIgnoreCase("status")) {
      Serial.printf("Current PWM Mode (manual test active)\r\n");
      return;
    }

    uint16_t pwm = input.toInt();
    if (pwm >= 0 && pwm <= 255) {
      ledcWrite(ledPin, pwm);
      Serial.printf("PWM set to: %d\r\n", pwm);
    } else {
      Serial.println("Invalid PWM value (0-255)");
    }
  }
}
