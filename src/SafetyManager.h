#ifndef SAFETY_MANAGER_H
#define SAFETY_MANAGER_H

#include <Arduino.h>

// Enum for the system's armed state.
enum ArmedState {
    DISARMED,
    ARMED,
    RANGE_OPEN // A state where the system is active but firing is disabled.
};

class SafetyManager {
public:
    // Constructor: Initializes the safety manager.
    SafetyManager(int armKeyPin, int tiltInterruptPin);

    // Setup: Initializes all safety features.
    void setup();

    // Loop: Must be called in the main loop to update safety status.
    void loop();

    // Arm the system. Returns true if successful.
    bool arm();

    // Disarm the system.
    void disarm();

    // Open the range. Firing is disabled.
    void openRange();

    // Close the range. Allows the system to be armed.
    void closeRange();

    // Returns the current armed state of the system.
    ArmedState getState();

    // Returns true if the system is currently armed.
    bool isArmed();

    // Returns true if the tilt sensor has been triggered.
    bool isTilted();

private:
    int _armKeyPin;         // Pin for the physical arming key.
    int _tiltInterruptPin;  // Pin for the MPU-6050 interrupt.

    ArmedState _state;      // The current armed state.
    bool _tiltTriggered;    // Flag to indicate if the tilt sensor has been triggered.
    unsigned long _lastActivityTime; // The time of the last arming or launch command.

    // The automatic disarm timeout in milliseconds (e.g., 10 minutes).
    const unsigned long AUTO_DISARM_TIMEOUT = 10 * 60 * 1000;

    // Initializes the hardware watchdog timer.
    void setupWatchdog();

    // Initializes the MPU-6050 tilt sensor.
    void setupTiltSensor();

    // Checks the status of the physical arming key.
    bool isArmKeyOn();
};

#endif // SAFETY_MANAGER_H
