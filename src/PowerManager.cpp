#include "PowerManager.h"

PowerManager::PowerManager(int batteryPin) {
    _batteryPin = batteryPin;
    _voltage = 0.0;
    _percentage = 0;
}

void PowerManager::setup() {
    // Set the ADC pin as an input.
    pinMode(_batteryPin, INPUT);
}

void PowerManager::loop() {
    // Read the raw ADC value.
    int rawValue = analogRead(_batteryPin);

    // Convert the ADC value to the voltage at the pin.
    float pinVoltage = (rawValue / ADC_MAX) * VOLTAGE_MAX;

    // Calculate the actual battery voltage using the voltage divider formula.
    // Vout = Vin * (R2 / (R1 + R2)) => Vin = Vout * ((R1 + R2) / R2)
    _voltage = pinVoltage * ((R1 + R2) / R2);

    // Calculate the battery percentage.
    _percentage = 100 * (_voltage - BATT_MIN_VOLTAGE) / (BATT_MAX_VOLTAGE - BATT_MIN_VOLTAGE);

    // Constrain the percentage between 0 and 100.
    if (_percentage > 100) {
        _percentage = 100;
    }
    if (_percentage < 0) {
        _percentage = 0;
    }
}

float PowerManager::getVoltage() {
    return _voltage;
}

int PowerManager::getPercentage() {
    return _percentage;
}
