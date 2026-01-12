#ifndef IGNITION_CONTROL_H
#define IGNITION_CONTROL_H

#include <Arduino.h>

const int NUM_CHANNELS = 16;

// Enum to represent the status of each launch pad.
enum PadStatus {
    UNKNOWN,      // Status has not been checked yet.
    DISCONNECTED, // No continuity detected.
    CONTINUITY_OK // Continuity detected, ready to fire.
};

class IgnitionControl {
public:
    // Constructor: Initializes the ignition control with the GPIO pins for the relays.
    IgnitionControl(const int relayPins[NUM_CHANNELS]);

    // Setup: Initializes the GPIO pins for the relays.
    void setup();

    // Performs a continuity check on all channels.
    void checkContinuity();

    // Fires a specific channel.
    // Takes the channel number (0-15) and a duration in milliseconds.
    // Returns true if the channel was fired successfully.
    // Note: This function should only be called after the system is confirmed to be in an ARMED state by the SafetyManager.
    bool fireChannel(int channel, int duration);

    // Returns the status of a specific channel.
    PadStatus getChannelStatus(int channel);

private:
    const int* _relayPins; // An array of GPIO pins connected to the relays (via optocouplers).
    PadStatus _channelStatus[NUM_CHANNELS]; // An array to store the status of each channel.
};

#endif // IGNITION_CONTROL_H
