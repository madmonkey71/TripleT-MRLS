# TripleT Model Rocket Launch System

## Disclaimer

**Rocketry is a dangerous activity. This project is a prototype and should be used with extreme caution. The creators of this project are not responsible for any damage or injury that may occur from the use of this system. Always follow safety guidelines and local laws when participating in rocketry activities.**

Project State: **ALPHA**
Not to be used in production. Use at your own risk

This is a prototype Launch System for Model Rockets

## Hardware
- The Hardware is the ESP32 and ESP8266 Series of Processors for the controllers.
  - Primary Hardware target is the ESP32-S3
- Relays to be used for Pyro Channels / ignition charges.
- Communication will be over 2.4Ghz Wifi
- Range needs to be at least 500m
  - Antenna will be needed
- There will be a web management frontend that runs on the ESP32 Processor
- There will be safety features builtin to stop accidental launches or issues while the range is closed.
  - A manual key system for arming the firing button
- The system should support up to 16 Launch pads (channel) per controller and be multi controller aware.
- The controller will need to continuity check each channel to ensure that the pad / ignitor is correctly connected using less than 10mW of power.
- The controller will need to be able to monitor the LiPo battery that is powering the system.
- An 11.7 Volt Lithium Polymer battery to power the system
- Onboard display (OLED or LCD) for at-a-glance system status.
- Buzzer for audible feedback (e.g., arming, countdowns, alerts).

### Optional Hardware
- Environmental sensors for Temperature, Humidity, and Barometric Pressure to log launch conditions.
- GPS module for logging launch site coordinates.
- Sensors should be connected using i2c where ever possible.
- SD Card for logging all system events and sensor data.

## Web Command Interface
- Some security or Authentication system so that only the Launch Director can initiate launches (in conjunection with the key)
- Range open and closed commands.
  - Range open should disable all launch commands and physically disable the ability to set off an ignitor
  - A Pin number securing range open and close button to prevent accidental opening of the range.
- A Launch command for each pad
- Display the continuity check for each pad / channel
- Real-time display of environmental sensor data (Temperature, Humidity, Pressure).
- Visual countdown timer before launch.
- A system status page showing battery level, WiFi connection, and GPS lock.
- Ability to create and execute simple launch sequences.
- An admin page that provide the ability to configure the system

