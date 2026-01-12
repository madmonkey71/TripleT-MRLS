#include <Arduino.h>
#include "PowerManager.h"
#include "SafetyManager.h"
#include "IgnitionControl.h"
#include "Communication.h"
#include "DisplayUI.h"

// --- Pin Definitions ---
#define PIN_BATTERY_SENSE 4
#define PIN_ARM_KEY 15
#define PIN_TILT_INTERRUPT 14
#define PIN_BUZZER 25

const int RELAY_PINS[NUM_CHANNELS] = {
    2, 5, 6, 7, 8, 9, 10, 11,
    12, 13, 16, 17, 18, 19, 20, 21
};

// --- Global Objects ---
PowerManager powerManager(PIN_BATTERY_SENSE);
SafetyManager safetyManager(PIN_ARM_KEY, PIN_TILT_INTERRUPT);
IgnitionControl ignitionControl(RELAY_PINS);
Communication comms;
DisplayUI displayUI(PIN_BUZZER);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("TripleT Model Rocket Launch System - Booting...");

  // Initialize all modules.
  powerManager.setup();
  safetyManager.setup();
  ignitionControl.setup();
  comms.setup(safetyManager, powerManager, ignitionControl); // Pass references to comms
  displayUI.setup();

  displayUI.shortBeep();
  Serial.println("System Initialized.");
}

void loop() {
  // Update core modules.
  powerManager.loop();
  safetyManager.loop();
  comms.loop(); // Handles WebSocket clients

  // --- Main Application Logic ---
  static unsigned long lastUpdateTime = 0;
  const int UPDATE_INTERVAL = 1000; // Update rate: 1 Hz

  if (millis() - lastUpdateTime > UPDATE_INTERVAL) {
    lastUpdateTime = millis();

    // 1. Check pad continuity.
    ignitionControl.checkContinuity();

    // 2. Update the OLED display.
    displayUI.updateDisplay(safetyManager, powerManager, ignitionControl);

    // 3. Send state update to all WebSocket clients.
    comms.sendStateUpdate();
  }

  // The firing sequence is now handled by the WebSocket command handler in Communication.cpp
}
