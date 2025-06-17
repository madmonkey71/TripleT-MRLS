# TripleT Model Rocket Launch System

## Disclaimer

**Rocketry is a dangerous activity. This project is a prototype and should be used with extreme caution. The creators of this project are not responsible for any damage or injury that may occur from the use of this system. Always follow safety guidelines and local laws when participating in rocketry activities.**

Project State: **ALPHA**
Not to be used in production. Use at your own risk

This is a prototype Launch System for Model Rockets

## Hardware
- Primary Hardware target is the ESP32-S3.

### Power System
- An 11.7 Volt Lithium Polymer battery to power the system.
- The controller will need to be able to monitor the LiPo battery that is powering the system.
- **Power Distribution Board (PDB):** To manage LiPo input and provide separate, regulated power rails for sensitive electronics (ESP32, sensors) and high-current ignition circuits.

### Ignition System
- Relays to be used for Pyro Channels / ignition charges.
- The system should support up to 16 Launch pads (channel) per controller and be multi-controller aware.
- The controller will need to continuity check each channel to ensure that the pad / ignitor is correctly connected using less than 10mW of power.

### Communication
- **Primary:** 2.4Ghz Wifi for the web management frontend. Range needs to be at least 500m (external antenna will be needed).
- **Redundant/Long-Range:** **LoRa (e.g., at 433MHz or 915MHz)** radio module for critical, long-range commands (e.g., launch, disarm) to ensure reliability.

### Core Safety Features
- **Hardware Watchdog Timer:** The built-in ESP32 watchdog must be used to automatically reboot the processor if the main software loop freezes.
- **Manual Key System:** A physical key switch for arming the firing button.
- **Ignition Circuit Isolation:** High-current pyro circuits must be **electrically isolated** from the microcontroller logic using **optocouplers** between GPIO pins and relay inputs. This is a critical safety feature to prevent high-power failures from damaging the controller.
- **Tilt/Motion Safety Lock:** An **accelerometer/gyroscope (e.g., MPU-6050)** to detect if the controller is knocked over or tilted beyond a safe angle, triggering an immediate and automatic disarm of all channels.
- **Automatic Disarm Timeout:** The system will automatically revert to a safe, "disarmed" state if it remains armed for a configurable period (e.g., 10 minutes) without any launch activity.

### Onboard Display and UI
- Onboard display (OLED or LCD) for at-a-glance system status. The display must show a dashboard with:
    - Armed/Disarmed Status (highly visible)
    - System Uptime
    - Battery Voltage and Percentage
    - Wi-Fi and LoRa Signal Strength
    - Summary of pad continuity status (e.g., "Pads: 16/16 OK")
- Buzzer for audible feedback (e.g., arming, countdowns, alerts).
- **Human-Readable Error Codes:** The system must provide clear, descriptive error messages on both the web UI and the onboard display (e.g., "Continuity Fail: Pad 3", "Low Battery: Disarming", "Comm Link Lost").

### Optional Hardware and Logging
- **Real-Time Clock (RTC):** A dedicated, battery-backed RTC module for accurate, persistent event timestamping.
- **SD Card:** For logging all system events, sensor data, and configuration.
- **Persistent Configuration:** All system settings (Wi-Fi credentials, number of active pads, etc.) should be saved to non-volatile storage (e.g., a JSON file on the SD card) to persist after a reboot.
- Environmental sensors for Temperature, Humidity, and Barometric Pressure to log launch conditions (I2C).
- GPS module for logging launch site coordinates (I2C).

## Web Command Interface
- Some security or Authentication system so that only the Launch Director can initiate launches (in conjunection with the key)
- **Two-Stage Web Arming:** The web interface will have a two-stage arming process. The user must first click an "Enable Arming" button, followed by a separate "Confirm Arm" button (e.g., requiring the user to type "ARM") to prevent accidental arming.
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

