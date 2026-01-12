#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include <Arduino.h>

class PowerManager {
public:
    // Constructor: Initializes the PowerManager with the ADC pin for battery monitoring.
    PowerManager(int batteryPin);

    // Setup: Initializes the ADC and any other required hardware.
    void setup();

    // Loop: This should be called in the main loop to perform periodic tasks.
    void loop();

    // Returns the current battery voltage.
    float getVoltage();

    // Returns the estimated battery percentage.
    int getPercentage();

private:
    int _batteryPin;      // The ADC pin connected to the voltage divider.
    float _voltage;       // The current battery voltage.
    int _percentage;      // The current battery percentage.

    // Constants for voltage calculation.
    // These will need to be calibrated based on the actual voltage divider resistors.
    const float R1 = 10000.0; // Resistance of R1 in ohms
    const float R2 = 10000.0; // Resistance of R2 in ohms
    const float ADC_MAX = 4095.0; // Maximum ADC value for ESP32
    const float VOLTAGE_MAX = 3.3; // ADC reference voltage

    // Constants for battery percentage calculation (for a 3S LiPo).
    const float BATT_MAX_VOLTAGE = 12.6; // Maximum voltage of a 3S LiPo
    const float BATT_MIN_VOLTAGE = 9.9;  // Minimum voltage of a 3S LiPo
};

#endif // POWER_MANAGER_H
