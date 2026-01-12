#include "IgnitionControl.h"

IgnitionControl::IgnitionControl(const int relayPins[NUM_CHANNELS]) {
    _relayPins = relayPins;
    for (int i = 0; i < NUM_CHANNELS; i++) {
        _channelStatus[i] = UNKNOWN;
    }
}

void IgnitionControl::setup() {
    // Initialize all relay pins as outputs and set them to LOW (off).
    for (int i = 0; i < NUM_CHANNELS; i++) {
        pinMode(_relayPins[i], OUTPUT);
        digitalWrite(_relayPins[i], LOW);
    }
}

void IgnitionControl::checkContinuity() {
    // This is a placeholder for the actual continuity check logic.
    // A real implementation would require a dedicated circuit for each channel
    // to safely pass a very small current (<10mW) through the igniter.
    // This circuit would typically use another set of GPIOs configured as analog inputs.
    // For now, we will simulate the behavior. Let's assume every other channel has continuity.
    for (int i = 0; i < NUM_CHANNELS; i++) {
        if (i % 2 == 0) {
            _channelStatus[i] = CONTINUITY_OK;
        } else {
            _channelStatus[i] = DISCONNECTED;
        }
    }
}

bool IgnitionControl::fireChannel(int channel, int duration) {
    // Check for valid channel number.
    if (channel < 0 || channel >= NUM_CHANNELS) {
        return false;
    }

    // Check if the channel has continuity. We should not fire a disconnected channel.
    if (_channelStatus[channel] != CONTINUITY_OK) {
        return false;
    }

    // --- Firing Sequence ---
    // 1. Activate the relay.
    digitalWrite(_relayPins[channel], HIGH);

    // 2. Wait for the specified duration.
    delay(duration);

    // 3. Deactivate the relay.
    digitalWrite(_relayPins[channel], LOW);

    // After firing, the igniter is consumed, so the continuity is lost.
    _channelStatus[channel] = DISCONNECTED;

    return true;
}

PadStatus IgnitionControl::getChannelStatus(int channel) {
    if (channel < 0 || channel >= NUM_CHANNELS) {
        return UNKNOWN;
    }
    return _channelStatus[channel];
}
