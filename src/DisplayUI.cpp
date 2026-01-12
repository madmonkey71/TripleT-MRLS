#include "DisplayUI.h"
// Note: An OLED library would be included here, e.g., #include <Adafruit_SSD1306.h>
// For this placeholder, we will simulate the display output via the Serial Monitor.

// --- Placeholder for a display object ---
// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DisplayUI::DisplayUI(int buzzerPin) {
    _buzzerPin = buzzerPin;
}

void DisplayUI::setup() {
    pinMode(_buzzerPin, OUTPUT);
    digitalWrite(_buzzerPin, LOW);
    setupDisplay();
}

void DisplayUI::setupDisplay() {
    // This is a placeholder for initializing the OLED display.
    // A real implementation would look like this:
    //
    // if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    //     Serial.println(F("SSD1306 allocation failed"));
    // }
    // display.clearDisplay();
    // display.setTextSize(1);
    // display.setTextColor(SSD1306_WHITE);
    // display.setCursor(0,0);
    // display.println("System Booting...");
    // display.display();
    Serial.println("OLED display initialized (placeholder).");
}

void DisplayUI::updateDisplay(const SafetyManager& safety, const PowerManager& power, const IgnitionControl& ignition) {
    // This function simulates drawing the UI to the screen by printing to the Serial Monitor.
    // A real implementation would use the display library's drawing functions.

    // --- Create the display content in a buffer ---
    String displayContent = "";
    displayContent += "========================\n";

    // Armed Status
    displayContent += "Status: ";
    switch (safety.getState()) {
        case DISARMED:
            displayContent += "DISARMED\n";
            break;
        case ARMED:
            displayContent += "** ARMED **\n";
            break;
        case RANGE_OPEN:
            displayContent += "RANGE OPEN\n";
            break;
    }

    // Battery Status
    displayContent += "Battery: " + String(power.getVoltage(), 1) + "V (" + String(power.getPercentage()) + "%)\n";

    // Pad Status Summary
    int continuityCount = 0;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        if (ignition.getChannelStatus(i) == CONTINUITY_OK) {
            continuityCount++;
        }
    }
    displayContent += "Pads: " + String(continuityCount) + "/" + String(NUM_CHANNELS) + " OK\n";

    // Wi-Fi and LoRa Status (placeholders)
    displayContent += "WiFi: AP Mode | LoRa: OK\n";

    displayContent += "========================\n";

    // "Draw" the content to the Serial Monitor.
    Serial.println("\n--- OLED Display Update ---");
    Serial.print(displayContent);
    Serial.println("---------------------------\n");
}

void DisplayUI::shortBeep() {
    digitalWrite(_buzzerPin, HIGH);
    delay(100);
    digitalWrite(_buzzerPin, LOW);
}

void DisplayUI::longBeep() {
    digitalWrite(_buzzerPin, HIGH);
    delay(500);
    digitalWrite(_buzzerPin, LOW);
}

void DisplayUI::armingBeep() {
    shortBeep();
    delay(100);
    shortBeep();
    delay(100);
    longBeep();
}
