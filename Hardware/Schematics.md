# TripleT Model Rocket Launch System - Schematics and Wiring

This document provides a detailed description of the hardware schematics and wiring for the TripleT Model Rocket Launch System. It is intended to be a human-readable guide that complements the KiCAD schematic files.

## 1. System Block Diagram

```
+-----------------+      +-------------------------+      +-------------------+
|   3S LiPo       |----->| Power Distribution Board|----->| 12V Rail (Relays) |
|   (11.1V)       |      | (PDB)                   |---+
+-----------------+      +-------------------------+   |
                                |                    |
                                v                    v
                         +----------------+     +----------------+
                         | 5V Regulator   |     | 3.3V Regulator |
                         +----------------+     +----------------+
                                |                    |
     +--------------------------+--------------------------+
     |                          |                          |
     v                          v                          v
+----------+      +-------------------------+      +----------------+
| ESP32-S3 |----->|   Sensors & Peripherals |      | LoRa Module    |
| (Core)   |      |   (5V / 3.3V)           |      | (3.3V)         |
+----------+      +-------------------------+      +----------------+
     |
     |  (GPIO)
     |
     +------------------------------------------------------------+
     |                                                            |
     v                                                            v
+------------------------+                             +------------------------+
| Optocoupler Isolation  |                             |   Safety Interlocks    |
| (16 Channels)          |                             |  (Key, Tilt Sensor)    |
+------------------------+                             +------------------------+
     |
     v
+------------------------+
|   Relay Bank (12V)     |
|   (16 Channels)        |
+------------------------+
     |
     v
 (To Igniters)
```

## 2. Component Connections

### 2.1. Power System

- **LiPo Battery:** A 3S LiPo battery is connected to the PDB's main input.
- **PDB:**
  - **Input:** XT60 connector for the LiPo battery.
  - **12V Output:** Powers the high-current side of the 16 ignition relays.
  - **5V Output:** Powers the ESP32-S3 board's `VIN` pin.
  - **3.3V Output:** Powers the LoRa module and can be used for other 3.3V sensors.
- **Battery Monitoring:** A voltage divider (e.g., two 10k resistors) is connected across the LiPo battery's main terminals. The center point is connected to a dedicated ADC pin on the ESP32 (e.g., `GPIO4`).

### 2.2. ESP32-S3 Core

- **Power:** The ESP32 is powered via its `5V` or `VIN` pin from the PDB's 5V rail.
- **I2C Bus (`SDA`, `SCL`):** A single I2C bus is used for multiple devices.
  - **OLED Display:** Connected to the I2C bus.
  - **BME280 Sensor:** Connected to the I2C bus.
  - **DS3231 RTC:** Connected to the I2C bus.
  - **MPU-6050 Gyro/Accel:** Connected to the I2C bus.
- **SPI Bus (`MOSI`, `MISO`, `SCK`, `CS`):**
  - **SD Card Reader:** Connected to the SPI bus with a dedicated Chip Select (`CS`) pin.
  - **LoRa Module:** Connected to the SPI bus with a dedicated Chip Select (`CS`) pin. An additional GPIO is required for the LoRa module's `RST` pin.
- **GPS Module:** Connected via UART (`TX`, `RX`) to a hardware serial port on the ESP32.

### 2.3. Ignition System (per channel)

This describes the connection for a single channel. This is replicated 16 times.

1. **ESP32 GPIO Pin:** A dedicated GPIO pin is used for each channel.
2. **Optocoupler Input:** The GPIO pin is connected to the input side of an optocoupler (e.g., 4N25) through a current-limiting resistor (e.g., 330 Ohm).
3. **Optocoupler Output:** The output side of the optocoupler is connected to the relay's coil. It switches the 12V power from the PDB to activate the relay.
4. **Relay:** The relay's common (`COM`) and normally open (`NO`) terminals are connected to the external ignition terminals for a launch pad.

**This electrical isolation is CRITICAL.** A failure in the high-power relay circuit will not damage the ESP32.

### 2.4. Safety Interlocks

- **Arming Key:** A physical key switch is connected in series with the main launch button (if a physical button exists) or is connected to a GPIO pin on the ESP32. When the key is "off", the circuit is broken, and the system cannot arm or fire, regardless of software commands.
- **Tilt Sensor (MPU-6050):** This is connected to the I2C bus. The firmware will continuously monitor the sensor's readings and will trigger a disarm state if the tilt angle exceeds a predefined threshold.

## 3. Example Pinout Table (Conceptual)

This is a conceptual pinout and may change during firmware development. All pins will be finalized in a `config.h` file.

| Pin       | Component                | Bus/Type    | Notes                                  |
|-----------|--------------------------|-------------|----------------------------------------|
| GPIO21    | OLED / BME280 / RTC      | I2C SDA     | Shared I2C Bus                         |
| GPIO22    | OLED / BME280 / RTC      | I2C SCL     | Shared I2C Bus                         |
| GPIO5     | SD Card                  | SPI CS      | Chip Select for SD Card                |
| GPIO18    | LoRa Module              | SPI CS      | Chip Select for LoRa                   |
| GPIO23    | LoRa / SD Card           | SPI MOSI    | Shared SPI Bus                         |
| GPIO19    | LoRa / SD Card           | SPI MISO    | Shared SPI Bus                         |
| GPIO13    | LoRa / SD Card           | SPI SCK     | Shared SPI Bus                         |
| GPIO16    | GPS Module               | UART RX     | Hardware Serial RX                     |
| GPIO17    | GPS Module               | UART TX     | Hardware Serial TX                     |
| GPIO4     | Battery Monitor          | ADC         | Analog input for voltage sensing       |
| GPIO15    | Arming Key Switch        | Digital In  | Physical safety interlock              |
| GPIO25    | Buzzer                   | Digital Out | For audible feedback                   |
| GPIO32-39 | Ignition Channels 1-8    | Digital Out | Connected to Optocouplers              |
| GPIOxx-yy | Ignition Channels 9-16   | Digital Out | (Requires using more GPIOs)            |
