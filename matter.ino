#include <Matter.h>
#include "config.h"

extern MatterDimmableLight DimmableLight;
extern MatterFan Fan;

extern void initializePreferences();
extern void initializeLightEndpoint();
extern void initializeFanEndpoint();
extern void printLightCommissioningState();
extern void printFanCommissioningState();
extern void updateLightAccessory();
extern void updateFanAccessory();
extern void decommissionLightAccessory();
extern void decommissionFanAccessory();

void initializeMatter() {
  // Initialize storage
  initializePreferences();

  // Initialize endpoints
  initializeLightEndpoint();
  initializeFanEndpoint();

  // Start Matter protocol - must be last after all endpoints are configured
  Matter.begin();
}

void initializePreferences() {
  // Load stored settings from flash memory
  matterPref.begin("MatterPrefs", false);
}

void printCommissioningStates() {
  printLightCommissioningState();
  printFanCommissioningState();
}

void updateAccessories() {
  updateLightAccessory();
  updateFanAccessory();
}

void decommisionAccessories() {
  decommissionLightAccessory();
  decommissionFanAccessory();
}

void checkMatterCommissioning(MatterDimmableLight& light, MatterFan& fan) {
  // If device was previously commissioned (paired to Matter hub), sync its state
  if (Matter.isDeviceCommissioned()) {
    Serial.println("Matter Node is commissioned and connected to Wi-Fi. Ready for use.");
    printCommissioningStates();
    updateAccessories();
  }
}

void waitForMatterCommissioning(MatterDimmableLight& light, MatterFan& fan) {
  // If device is not commissioned (paired to Matter hub), wait for it
  if (!Matter.isDeviceCommissioned()) {
    Serial.println("");
    Serial.println("Matter Node is not commissioned yet.");
    Serial.println("Initiate the device discovery in your Matter environment.");
    Serial.println("Commission it to your Matter hub with the manual pairing code or QR code");
    Serial.printf("Manual pairing code: %s\r\n", Matter.getManualPairingCode().c_str());
    Serial.printf("QR code URL: %s\r\n", Matter.getOnboardingQRCodeUrl().c_str());

    // Loop until commissioning completes (blocking)
    uint32_t timeCount = 0;
    while (!Matter.isDeviceCommissioned()) {
      delay(100);
      // Print status every 5 seconds
      if ((timeCount++ % 50) == 0) {
        Serial.println("Matter Node not commissioned yet. Waiting for commissioning.");
      }
    }

    // Commissioning done - sync state
    printCommissioningStates();
    updateAccessories();
    Serial.println("Matter Node is commissioned and connected to Wi-Fi. Ready for use.");
  }
}

void handleButtonDecommissioning() {
  // Remove device from Matter hub (unpair) - triggered by long button press
  extern uint32_t button_time_stamp;
  decommisionAccessories();
  // Remove from Matter hub
  Matter.decommission();
  // Reset timestamp to prevent repeated decommissioning
  button_time_stamp = millis();
}
