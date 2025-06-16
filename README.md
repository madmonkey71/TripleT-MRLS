# TripleT Model Rocket Launch System

## Disclaimer

**Rocketry is a dangerous activity. This project is a prototype and should be used with extreme caution. The creators of this project are not responsible for any damage or injury that may occur from the use of this system. Always follow safety guidelines and local laws when participating in rocketry activities.**

This is a prototype Launch System for Model Rockets. This document outlines the hardware and software features of the system.

## Features

### Hardware
- **Processors:** ESP32 and ESP8266 Series for controllers.
- **Ignition:** Relays for Pyro Channels / ignition charges.
- **Communication:** 2.4Ghz Wifi with a range of at least 500m.
- **Power:** 11.7 Volt Lithium Polymer battery.
- **Safety:**
    - Key system for arming the firing button.
    - Continuity check for each channel (< 10mW).
- **Capacity:** Supports up to 16 launch pads per controller and is multi-controller aware.
- **Monitoring:**
    - LiPo battery monitoring.
    - Environmental sensors for Temperature, Humidity, and Barometric Pressure.
    - GPS module for logging launch site coordinates.
- **Feedback:**
    - Onboard display (OLED or LCD) for at-a-glance system status.
    - Buzzer for audible feedback (e.g., arming, countdowns, alerts).
- **Data Logging:** SD Card for logging all system events and sensor data.


### Web Command Interface
The system is managed through a web frontend running on the ESP32.

- **Range Control:**
    - Range open and closed commands.
    - PIN number securing range commands to prevent accidental changes.
- **Launch Control:**
    - Individual launch command for each pad.
    - Ability to create and execute simple launch sequences.
    - Visual countdown timer before launch.
- **System Status & Display:**
    - Display of continuity check for each pad/channel.
    - Real-time display of environmental sensor data.
    - A system status page showing battery level, WiFi connection, and GPS lock.

## Hardware Schematic
A hardware schematic can be found in the KiCAD Project located in the Hardware folder.

## Project Plan
The full project plan and definition can be found in `Project_Definition.md`.
