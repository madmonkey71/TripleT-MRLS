#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

// Forward declarations to avoid circular dependencies
class SafetyManager;
class PowerManager;
class IgnitionControl;

class Communication {
public:
    Communication();
    void setup(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition);
    void loop();
    void sendStateUpdate();

private:
    const char* _ssid = "TripleT_MRLS";
    const char* _password = "launchday";

    void setupWifiAP();
    void setupWebServer(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition);
    void setupWebSocket(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition);
    void handleWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
    void onWebSocketMessage(uint8_t *data, size_t len, AsyncWebSocketClient *client, SafetyManager& safety, IgnitionControl& ignition);
};

#endif // COMMUNICATION_H
