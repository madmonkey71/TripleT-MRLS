# TripleT Model Rocket Launch System
This is a prototype Launch System for Model Rockets

## Hardware
- The Hardware is the ESP32 and ESP8266 Series of Processors for the controllers.
- Relays to be used for Pyro Channels / ignition charges.
- Communication will be over 2.4Ghz Wifi
- Range needs to be at least 500m
- There will be a web management frontend that runs on the ESP32 Processor
- There will be safety features builtin to stop accidental launches or issues while the range is closed.
  - A key system for arming the firing button
- The system should support up to 16 Launch pads (channel) per controller and be multi controller aware.
- The controller will need to continuity check each channel to ensure that the pad / ignitor is correctly connected using less than 10mW of power.
- The controller will need to be able to monitor the LiPo battery that is powering the system.
- An 11.7 Volt Lithium Polymer battery to power the system

## Command Interface
- Range open and closed commands
  - A Pin number securing range open and close button to prevent accidental opening of the range.
- A Launch command for each pad
- Display the continuity check for each pad / channel

