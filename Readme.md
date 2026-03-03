
# ⚡ Mixed-Signal Real-Time Embedded Motor Control Platform

**Microcontroller:** TM4C123GH6PM (ARM Cortex-M4)\
**Firmware Type:** Bare-Metal (Interrupt-Driven)\
**Design Scope:** Full Custom Hardware + Firmware Integration

------------------------------------------------------------------------

## 📌 Project Summary

This project implements a complete mixed-signal embedded control system
designed, built, and validated from schematic to physical hardware. The
system integrates real-time motor control, analog signal acquisition,
multi-device I2C communication, UART diagnostics, and a high-power
H-bridge motor driver stage.

------------------------------------------------------------------------

# 📄 Hardware Design Files

## 🔹 Schematic (PDF)

Full hardware schematic (Rev 002):

![EL507-202508-LAB012-SCH001](https://github.com/user-attachments/assets/fe6662da-7cf1-452d-a3e2-1bc8ac3dfa4b)



This schematic defines: - Power regulation architecture (+12V, +5V,
+3.3V, +3.3VA) - TM4C123GH6PM microcontroller integration - Analog
signal conditioning stage - I2C bus devices (RTC, IO Expander, DAC) -
LCD interface - H-bridge motor driver stage (HIP4082 + MOSFETs) - QEI
encoder interface - Decoupling and power filtering networks

------------------------------------------------------------------------

## 🔹 Physical Breadboard Implementation

Actual hardware build of the schematic:

![RUID717566d423e942928adba23f8b0fff25](https://github.com/user-attachments/assets/157b2e78-109c-4dea-84b1-c9b9e0858f8c)


The breadboard implementation demonstrates: - Complete multi-rail power
distribution - Analog and digital domain separation - H-bridge motor
driver wiring - LCD interface integration - I2C peripheral grouping -
Real motor testing and validation

This validates the schematic design in physical hardware.

------------------------------------------------------------------------

# 🧱 System Architecture

### Core Subsystems

-   ARM Cortex-M4 Microcontroller (TM4C123GH6PM)
-   Multi-rail Power Regulation
-   Analog Front-End (Op-Amps + Temperature Sensor)
-   I2C Peripheral Network (RTC, IO Expander, DAC)
-   UART Command-Line Interface
-   LCD Display Interface
-   H-Bridge Motor Driver
-   Quadrature Encoder Feedback (QEI)

------------------------------------------------------------------------

# 🔋 Power Architecture

-   +12V rail for motor stage
-   +5V rail for logic/interface
-   +3.3V digital rail
-   +3.3VA analog rail
-   MCP1700 LDO regulators
-   Dedicated analog ground (GNDA)
-   Bulk capacitors for motor stabilization

Demonstrates proper mixed-signal power isolation and integrity planning.

------------------------------------------------------------------------

# 🧠 Microcontroller Features Used

-   PWM generation (M0PWM0 / M0PWM1)
-   QEI speed feedback decoding
-   Multi-channel ADC sampling
-   I2C master communication
-   UART serial interface
-   SysTick interrupt scheduling
-   External crystal oscillators
-   JTAG programming interface

------------------------------------------------------------------------

# 🎛 Motor Control System

### Manual Mode

-   Fixed RPM setpoints via UART commands

### Automatic Mode

-   ADC voltage input scales motor speed
-   QEI feedback monitors real-time RPM
-   PWM dynamically controls motor output

Motor driver implemented using: - HIP4082 high/low-side driver -
N-channel MOSFET H-bridge - Bootstrap capacitors - Flyback diodes - 12V
motor supply filtering

------------------------------------------------------------------------

# 📊 Data Acquisition

-   Multi-channel ADC sampling
-   Voltage scaling and conversion
-   Ambient temperature measurement
-   Internal temperature monitoring
-   Periodic sampling via SysTick

------------------------------------------------------------------------

# 🔌 I2C Peripheral Network

Devices on shared I2C bus:

-   MCP7940M -- Real-Time Clock
-   PCF8574A -- I/O Expander
-   MAX518 -- DAC

Features: - 4.7kΩ pull-up resistors - Multi-device addressing - Periodic
polling and updates

------------------------------------------------------------------------

# 🖥 UART Command Interface

Supports:

-   Date and time display
-   Motor speed reporting (RPM)
-   Temperature monitoring
-   Mode switching (Manual / Automatic)
-   LED toggling
-   System reset
-   Help menu

------------------------------------------------------------------------

# ⏱ Firmware Architecture

-   Bare-metal interrupt-driven design
-   SysTick-based periodic scheduling
-   Event flag synchronization
-   Modular driver abstraction
-   Low-power main loop using `__wfi()`

Ensures deterministic real-time performance.

------------------------------------------------------------------------

# 🛠 Engineering Concepts Demonstrated

-   ARM Cortex-M4 architecture
-   Register-level programming
-   Interrupt-driven firmware design
-   ADC signal conditioning
-   PWM motor control
-   Closed-loop feedback using QEI
-   Multi-device I2C bus communication
-   Mixed-signal hardware design
-   Hardware bring-up and validation

------------------------------------------------------------------------

# 🚀 Future Improvements

-   PID-based motor control
-   RTOS integration
-   CAN/Ethernet communication
-   PCB implementation
-   Data logging to flash
-   Bootloader support

------------------------------------------------------------------------

# 🏁 Conclusion

This project represents a complete embedded hardware--firmware system
integrating sensing, communication, user interface, and high-power motor
control. It demonstrates industry-level embedded engineering capability
from schematic design to physical system validation.
