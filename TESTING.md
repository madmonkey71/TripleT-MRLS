# TripleT Model Rocket Launch System - Integration Test Plan

## 1. Introduction

This document outlines the test plan for the TripleT Model Rocket Launch System. The purpose of this plan is to provide a structured approach to verify and validate the functionality of the integrated system, with a primary focus on its safety features.

**Disclaimer:** These tests should be performed in a safe, controlled environment. At no point should a live igniter be connected to the system unless it is properly installed in a rocket motor on a launch pad, and all safety precautions have been followed. For most of these tests, a simple multimeter or an LED with a resistor can be used to check for voltage on the relay outputs.

## 2. Test Environment

- **Hardware:**
  - Assembled TripleT MRLS controller.
  - 3S LiPo battery (fully charged).
  - Laptop/PC with Wi-Fi and a modern web browser.
  - (Optional but Recommended) Multimeter or 16 LEDs with current-limiting resistors to simulate igniters.
- **Firmware:** The latest compiled version from the `dev` branch.
- **Software:** A web browser (Chrome, Firefox, Safari).

## 3. Test Cases

### 3.1. Power System Tests

| Test ID | Test Case                  | Steps                                                                                                   | Expected Result                                                                                               |
|---------|----------------------------|---------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------|
| `PWR-01`  | System Boot-up             | 1. Connect the LiPo battery.                                                                            | The system boots up. The OLED display shows the boot screen, and the buzzer emits a short beep.                 |
| `PWR-02`  | Battery Voltage Reading    | 1. Boot the system. <br> 2. Connect to the web UI. <br> 3. Compare the voltage on the UI to a multimeter reading. | The voltage displayed on the OLED and web UI should be within +/- 0.2V of the actual battery voltage.         |

### 3.2. Safety Feature Tests

| Test ID | Test Case                  | Steps                                                                                                   | Expected Result                                                                                               |
|---------|----------------------------|---------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------|
| `SF-01`   | Physical Arming Key        | 1. With the arming key in the "OFF" position, try to arm the system via the web UI.                     | The system should refuse to arm. The state on the UI should remain "DISARMED".                                |
| `SF-02`   | Tilt Sensor (Simulation)   | 1. Boot the system normally. <br> 2. Tilt the controller by more than 45 degrees. <br> 3. Try to arm.        | The system should enter a "TILTED" or "ERROR" state and refuse to arm. If armed, it should immediately disarm. |
| `SF-03`   | Auto-Disarm Timeout        | 1. Arm the system via the web UI. <br> 2. Do not perform any actions for 11 minutes.                      | The system should automatically disarm itself after 10 minutes. The state on the UI should change to "DISARMED". |
| `SF-04`   | Two-Stage Web Arming       | 1. Click the "Arm" button on the web UI. <br> 2. In the modal, type something other than "ARM". <br> 3. Click "Confirm". <br> 4. Repeat, but type "ARM". | The system should not arm on the incorrect input. The system should arm correctly on the correct input.       |

### 3.3. Web Interface and Firing Tests

| Test ID | Test Case                  | Steps                                                                                                   | Expected Result                                                                                               |
|---------|----------------------------|---------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------|
| `WEB-01`  | Connect to Web UI          | 1. Find the "TripleT_MRLS" Wi-Fi network and connect. <br> 2. Navigate to the IP address in a browser.    | The web interface loads correctly. The status dashboard shows real-time data.                                 |
| `WEB-02`  | Continuity Check Display   | 1. Connect a simulated igniter (LED/resistor) to a channel. <br> 2. Observe the web UI.                | The corresponding pad on the UI should change state to "CONTINUITY_OK".                                       |
| `WEB-03`  | Fire Channel (Disarmed)    | 1. With the system disarmed, click on a pad with continuity in the web UI.                              | The system should not fire. No voltage should be detected on the relay output.                                |
| `WEB-04`  | Fire Channel (Armed)       | 1. Arm the system correctly. <br> 2. Click on a pad with continuity in the web UI.                     | The corresponding relay should activate for the specified duration (e.g., 500ms). The UI should show "FIRING". |
| `WEB-05`  | Post-Fire Status           | 1. After firing a channel, observe its status.                                                          | The channel's status should revert to "DISCONNECTED" as the igniter is now considered spent.                  |

## 4. Test Execution

- All tests should be executed in the order they are presented.
- The "Actual Result" column should be filled in during testing.
- Any discrepancies between the "Expected Result" and "Actual Result" should be documented as a bug.
