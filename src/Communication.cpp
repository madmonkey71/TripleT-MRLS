#include "Communication.h"
#include <WiFi.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "SafetyManager.h"
#include "PowerManager.h"
#include "IgnitionControl.h"

// --- Global Web Server Objects ---
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Communication::Communication() {}

void Communication::setup(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition) {
    setupWifiAP();

    // Initialize SPIFFS to serve web files
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    setupWebServer(safety, power, ignition);
    setupWebSocket(safety, power, ignition);
    server.begin();
    Serial.println("Web server started.");
}

void Communication::loop() {
    ws.cleanupClients();
}

void Communication::setupWifiAP() {
    WiFi.softAP(_ssid, _password);
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void Communication::setupWebServer(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition) {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", "text/html");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "application/javascript");
    });
}

void Communication::onWebSocketMessage(uint8_t *data, size_t len, AsyncWebSocketClient *client, SafetyManager& safety, IgnitionControl& ignition) {
    JsonDocument doc;
    deserializeJson(doc, (char*)data);

    String command = doc["command"];
    Serial.println("Received WebSocket command: " + command);

    if (command == "arm") {
        if (safety.arm()) {
          // Optional: Add arming beep from DisplayUI if accessible
        }
    } else if (command == "disarm") {
        safety.disarm();
    } else if (command == "fire") {
        int padIndex = doc["pad"];
        // Duration is hardcoded for now, could be made configurable
        ignition.fireChannel(padIndex, 500);
    }
    // Add handlers for range open/close here...

    // After any command, send an immediate state update back to the client(s)
    sendStateUpdate();
}

void Communication::handleWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    // This is a static context, so we can't access `this`. We'll need a way to get back to our instance.
    // A more robust solution might use a singleton or pass a pointer in the websocket's context.
    // For this project, we'll rely on the global `comms` object to call the update function.
    extern Communication comms;
    extern SafetyManager safetyManager;
    extern IgnitionControl ignitionControl;

    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        // Send the current state to the newly connected client
        comms.sendStateUpdate();
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
    } else if (type == WS_EVT_DATA) {
        onWebSocketMessage(data, len, client, safetyManager, ignitionControl);
    }
}

void Communication::setupWebSocket(SafetyManager& safety, PowerManager& power, IgnitionControl& ignition) {
    // C-style function pointers require a careful setup.
    // Using a lambda to capture the context would be ideal, but ESPAsyncWebServer's C-style API makes this tricky.
    // We will use a static-like approach within handleWebSocketEvent.
    ws.onEvent(handleWebSocketEvent);
    server.addHandler(&ws);
}

void Communication::sendStateUpdate() {
    // This function needs access to the main objects. A better design would be to pass them in.
    // For now, we'll use the global objects.
    extern SafetyManager safetyManager;
    extern PowerManager powerManager;
    extern IgnitionControl ignitionControl;

    JsonDocument doc;

    // Get the state string
    String stateStr;
    switch(safetyManager.getState()){
        case DISARMED: stateStr = "DISARMED"; break;
        case ARMED: stateStr = "ARMED"; break;
        case RANGE_OPEN: stateStr = "RANGE_OPEN"; break;
    }
    doc["state"] = stateStr;

    JsonObject battery = doc.createNestedObject("battery");
    battery["voltage"] = powerManager.getVoltage();
    battery["percentage"] = powerManager.getPercentage();

    JsonArray pads = doc.createNestedArray("pads");
    for (int i = 0; i < NUM_CHANNELS; i++) {
        pads.add(ignitionControl.getChannelStatus(i) == CONTINUITY_OK ? "CONTINUITY_OK" : "DISCONNECTED");
    }

    String jsonString;
    serializeJson(doc, jsonString);

    ws.textAll(jsonString);
}
