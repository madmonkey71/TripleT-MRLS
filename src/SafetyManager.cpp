#include "SafetyManager.h"
#include <esp_task_wdt.h> // For the watchdog timer

// Note: The MPU-6050 library would be included here, e.g., #include <Adafruit_MPU6050.h>

SafetyManager::SafetyManager(int armKeyPin, int tiltInterruptPin) {
    _armKeyPin = armKeyPin;
    _tiltInterruptPin = tiltInterruptPin;
    _state = DISARMED;
    _tiltTriggered = false;
    _lastActivityTime = 0;
}

void SafetyManager::setup() {
    pinMode(_armKeyPin, INPUT_PULLUP); // Assuming a switch that pulls to ground when on.
    // Tilt sensor interrupt pin would be configured here.

    setupWatchdog();
    setupTiltSensor();

    _state = DISARMED; // Ensure the system starts in a safe state.
}

void SafetyManager::loop() {
    // 1. Feed the watchdog to prevent a reboot.
    esp_task_wdt_reset();

    // 2. Check for tilt sensor trigger.
    // This is a critical safety feature. If the controller is tilted, disarm immediately.
    // A real implementation would check an interrupt flag set by the MPU-6050.
    // For now, we will simulate this with a placeholder.
    // if (digitalRead(_tiltInterruptPin) == LOW) {
    //     _tiltTriggered = true;
    // }

    if (_tiltTriggered) {
        disarm();
        // A real implementation would also log this event.
        return; // Halt further processing until the tilt is cleared.
    }

    // 3. Check for automatic disarm timeout.
    if (isArmed() && (millis() - _lastActivityTime > AUTO_DISARM_TIMEOUT)) {
        disarm();
        // A real implementation would log this event and notify the user.
    }

    // 4. A physical key turned to "off" should always disarm the system.
    if (!isArmKeyOn() && _state != DISARMED) {
        disarm();
    }
}

bool SafetyManager::arm() {
    // The system can only be armed if it is currently disarmed, the range is closed,
    // the physical key is on, and the system is not tilted.
    if (_state == DISARMED && isArmKeyOn() && !_tiltTriggered) {
        _state = ARMED;
        _lastActivityTime = millis();
        // A real implementation would provide audible feedback (buzzer).
        return true;
    }
    return false;
}

void SafetyManager::disarm() {
    if (_state != DISARMED) {
        _state = DISARMED;
        // A real implementation would provide audible feedback.
    }
}

void SafetyManager::openRange() {
    // The range can only be opened if the system is disarmed.
    if (_state == DISARMED) {
        _state = RANGE_OPEN;
    }
}

void SafetyManager::closeRange() {
    // Closing the range returns the system to the disarmed state.
    if (_state == RANGE_OPEN) {
        _state = DISARMED;
    }
}

ArmedState SafetyManager::getState() {
    return _state;
}

bool SafetyManager::isArmed() {
    return _state == ARMED;
}

bool SafetyManager::isTilted() {
    return _tiltTriggered;
}

void SafetyManager::setupWatchdog() {
    // Set up the watchdog timer for a 5-second timeout.
    // If the main loop freezes for longer than this, the ESP32 will reboot.
    esp_task_wdt_init(5, true);
    esp_task_wdt_add(NULL);
}

void SafetyManager::setupTiltSensor() {
    // In a real implementation, this would involve:
    // 1. Initializing the MPU-6050 library.
    // 2. Configuring the sensor's sensitivity.
    // 3. Setting up the motion detection interrupt to trigger on a significant tilt.
    // For now, this is a placeholder.
    _tiltTriggered = false; // Assume the controller starts level.
}

bool SafetyManager::isArmKeyOn() {
    // We use INPUT_PULLUP, so the pin is LOW when the key is on (circuit closed).
    return digitalRead(_armKeyPin) == LOW;
}
