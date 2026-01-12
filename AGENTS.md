# TripleT Model Rocket Launch System - Agent Guidelines

This document outlines the development standards, coding conventions, and safety-critical guidelines that must be followed by any AI agent working on this project. The primary goal is to ensure the safety, reliability, and predictability of the launch control system.

## 1. Safety First (The "Prime Directive")

**All changes must be evaluated for their impact on safety.** This is the most important rule.

- **Never Disable Safety Features:** Under no circumstances should any existing safety feature be disabled or bypassed in the main branch. This includes the hardware watchdog, tilt sensor, arming timeouts, and ignition circuit isolation.
- **Redundancy and Failsafes:** Where possible, design for redundancy. The system should default to a "safe" (disarmed) state in the event of any failure (e.g., power loss, communication loss, software crash).
- **Justify Safety-Related Changes:** Any modification to a safety-critical component (e.g., arming logic, ignition control, power management) must be explicitly justified in the commit message, explaining how the change maintains or enhances safety.

## 2. Hardware and Coding Conventions

- **Target Platform:** All firmware code must be written for the ESP32-S3 and be compatible with the Arduino framework unless otherwise specified.
- **Pin Assignments:** All pin assignments must be managed in a central configuration file (e.g., `config.h`). Do not hardcode pin numbers in the application logic.
- **Electrical Isolation:** Remember that the ignition circuits are electrically isolated via optocouplers. The code should treat them as simple GPIO outputs, but all design decisions must respect this hardware separation.
- **Low Power Continuity:** The continuity check must not exceed 10mW of power. Code implementing this feature must be written to use minimal current.
- **Resource Management:** Be mindful of the ESP32's limited resources. Optimize code for memory usage and performance. Avoid blocking calls in the main loop; use asynchronous patterns where possible.

## 3. Web Interface Development

- **Security:** The web interface is a primary control surface. All actions that change the state of the system (e.g., arming, opening the range) must be protected. The two-stage arming process and PIN protection for range commands are a minimum requirement.
- **Clarity and Simplicity:** The UI must be clear, simple, and unambiguous. Critical status information (e.g., "ARMED", "DISARMED", "Continuity OK") should be the most prominent information on the screen.
- **Asynchronous Updates:** The web UI should use asynchronous JavaScript (e.g., Fetch API, WebSockets) to get real-time status updates from the ESP32 without requiring page reloads.

## 4. Code Quality and Verification

- **Code Comments:** Document all non-obvious code, especially safety-critical sections. Explain *why* the code is written the way it is, not just *what* it does.
- **Verification:** After every code modification, you must perform a verification step.
  - For firmware changes, this means recompiling the code to ensure it builds without errors.
  - For web interface changes, this means validating the HTML/CSS/JS syntax.
- **Testing:** While a full hardware-in-the-loop test is not always possible, you must write and run unit tests for pure logic functions whenever feasible.

By adhering to these guidelines, we can ensure that the TripleT Model Rocket Launch System is developed in a safe, reliable, and responsible manner.
