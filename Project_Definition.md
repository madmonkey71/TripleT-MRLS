# TripleT Model Rocket Launch System

## Disclaimer

**Rocketry is a dangerous activity. This project is a prototype and should be used with extreme caution. The creators of this project are not responsible for any damage or injury that may occur from the use of this system. Always follow safety guidelines and local laws when participating in rocketry activities.**

Project State: **ALPHA**
Not to be used in production. Use at your own risk

This is a prototype Launch System for Model Rockets

## Hardware
- Primary Hardware target is the ESP32-S3.

### Power System
- **Power Source:** 3S (11.1V nominal) Lithium Polymer (LiPo) battery with a capacity of at least 2200mAh.
- **Battery Monitoring:** A voltage divider circuit connected to an ADC pin on the ESP32 to monitor the LiPo battery voltage.
- **Power Distribution Board (PDB):** A custom or off-the-shelf PDB to manage LiPo input and provide clean, regulated power:
    - **12V Rail:** For high-current ignition relays.
    - **5V Rail:** For the ESP32-S3 board and other 5V peripherals.
    - **3.3V Rail:** For sensors and LoRa module.

### Ignition System
- Relays to be used for Pyro Channels / ignition charges.
- The system should support up to 16 Launch pads (channel) per controller and be multi-controller aware.
- The controller will need to continuity check each channel to ensure that the pad / ignitor is correctly connected using less than 10mW of power.

### Communication
- **Primary (Wi-Fi):** 2.4GHz Wi-Fi for the web management frontend. An ESP32-S3 with an external antenna connector is required to achieve the 500m range requirement.
- **Redundant/Long-Range (LoRa):** A HopeRF RFM95/96 module operating at 915MHz (US) or 433MHz (EU) for critical, long-range commands (launch, disarm).

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
- **Real-Time Clock (RTC):** A DS3231 or PCF8523 battery-backed RTC module for accurate, persistent event timestamping.
- **SD Card:** A MicroSD card slot for logging all system events, sensor data, and configuration.
- **Persistent Configuration:** All system settings will be saved in a `config.json` file on the SD card to persist after a reboot.
- **Environmental Sensors:** A BME280 sensor connected via I2C to log temperature, humidity, and barometric pressure.
- **GPS Module:** A u-blox NEO-6M or similar GPS module for logging launch site coordinates and providing a time source for the RTC.

## Firmware Modules

The firmware will be developed in a modular fashion to improve organization and maintainability.

- **`PowerManager.cpp/.h`:** Handles reading the battery voltage and calculating the remaining percentage.
- **`SafetyManager.cpp/.h`:** Manages all core safety features: watchdog, tilt sensor, arming key, and disarm timeouts.
- **`IgnitionControl.cpp/.h`:** Manages continuity checks and the firing sequence for the 16 relay channels.
- **`Communication.cpp/.h`:** Handles both Wi-Fi and LoRa communications.
- **`DisplayUI.cpp/.h`:** Controls the OLED display and the buzzer.
- **`DataLogger.cpp/.h`:** Manages logging to the SD card and reading/writing configuration.
- **`SensorManager.cpp/.h`:** Reads and processes data from the BME280 and GPS modules.

## Web Command Interface / Software Architecture

The web interface will be a single-page application (SPA) served directly from the ESP32.

- **Backend:** An `ESPAsyncWebServer` instance will serve the frontend files and provide a WebSocket for real-time, bidirectional communication.
- **Frontend:** The frontend will be built with plain HTML, CSS, and JavaScript. It will communicate with the backend via WebSockets to send commands and receive real-time status updates.
- **Security:**
  - **Authentication:** A simple username/password authentication system will be implemented to protect the main control interface.
  - **Two-Stage Arming:** The web interface will enforce a two-stage arming process.
  - **PIN Protection:** Range open/close commands will be protected by a user-configurable PIN.
- **Features:**
  - A real-time system status dashboard (battery, connectivity, sensor data).
  - A launch control panel with individual pad status and launch buttons.
  - A configuration page for setting Wi-Fi credentials, arming timeouts, and other parameters.
  - A log viewer to display past events from the SD card.
  - The ability to create and save simple, multi-step launch sequences.

