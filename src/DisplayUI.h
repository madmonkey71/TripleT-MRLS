#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include "SafetyManager.h" // For ArmedState enum
#include "PowerManager.h"  // For PowerManager class
#include "IgnitionControl.h" // For IgnitionControl class

class DisplayUI {
public:
    // Constructor: Initializes the display UI manager.
    DisplayUI(int buzzerPin);

    // Setup: Initializes the display and buzzer.
    void setup();

    // Updates the display with the latest system status.
    // This should be called periodically.
    void updateDisplay(const SafetyManager& safety, const PowerManager& power, const IgnitionControl& ignition);

    // --- Buzzer Functions ---
    // A short beep for acknowledgements.
    void shortBeep();

    // A longer beep for alerts or warnings.
    void longBeep();

    // A specific pattern for the arming sequence.
    void armingBeep();

private:
    int _buzzerPin;

    // Initializes the OLED display.
    void setupDisplay();
};

#endif // DISPLAY_UI_H
