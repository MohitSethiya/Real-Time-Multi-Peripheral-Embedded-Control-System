# Real-Time Embedded Motor Control & Monitoring System

**Platform:** TM4C123GH6PM (ARM Cortex-M4)

**Language:** C (Bare-Metal)

**Architecture:** Interrupt-Driven, Modular Driver-Based Design

------------------------------------------------------------------------

## Overview

This project implements a real-time embedded control and monitoring
system on the TM4C123GH6PM microcontroller. The system integrates
multiple hardware peripherals including ADC, UART, I2C devices, PWM
motor driver, QEI encoder feedback, LCD interface, and external I/O
expanders.

The firmware is designed using a modular driver architecture with
interrupt-driven task handling. It supports both manual and automatic
control modes and demonstrates closed-loop motor control with real-time
sensor monitoring.

------------------------------------------------------------------------

## Key Features

-   Real-time motor speed control with QEI feedback
-   Multi-channel ADC data acquisition
-   Manual and automatic control modes
-   UART-based command-line interface (CLI)
-   I2C multi-device integration (RTC, IO Expander, DAC)
-   Periodic task scheduling using SysTick
-   Interrupt-driven architecture
-   Modular peripheral drivers

------------------------------------------------------------------------

## System Architecture

### Core Components

-   **TM4C123GH6PM Microcontroller**
-   **Motor Control Subsystem**
    -   PWM output
    -   QEI speed feedback
-   **Sensor Subsystem**
    -   Voltage measurement
    -   Ambient temperature
    -   Internal temperature
-   **Communication Subsystem**
    -   UART CLI
    -   I2C (RTC, IO Expander, DAC)
-   **User Interface**
    -   LCD display
    -   Push buttons with software debouncing

------------------------------------------------------------------------

## Firmware Design

### Initialization Layer

-   System clock configuration
-   SysTick timer setup
-   Peripheral initialization (ADC, UART, I2C, QEI, PWM, LCD)
-   Driver-level configuration

### Real-Time Execution Model

The system uses:

-   SysTick interrupts for periodic scheduling
-   Event flag-based processing
-   Low-power wait state (`__wfi()`) in the main loop

This ensures deterministic behavior and efficient CPU utilization.

------------------------------------------------------------------------

## Motor Control Strategy

### Manual Mode

-   User selects predefined RPM values via UART input (0--9 keys)
-   Speed is directly mapped to motor setpoint

### Automatic Mode

-   Motor speed is derived from analog input voltage
-   QEI feedback provides real-time speed monitoring
-   PWM duty cycle adjusts dynamically

------------------------------------------------------------------------

## Data Acquisition

-   Multi-channel ADC sampling
-   Voltage scaling and conversion
-   External and internal temperature calculations
-   Automatic triggering via SysTick

Sensor outputs are accessible via UART CLI and LCD display.

------------------------------------------------------------------------

## UART Command Interface

The system supports:

-   Date & time display (RTC integration)
-   Motor speed reporting (RPM)
-   Temperature monitoring
-   Mode switching (Manual/Automatic)
-   LED control
-   System reset commands
-   Help menu

------------------------------------------------------------------------

## Embedded Concepts Demonstrated

-   ARM Cortex-M architecture
-   Register-level peripheral configuration
-   Interrupt-driven firmware design
-   SysTick-based task scheduling
-   ADC triggering and data processing
-   PWM motor control
-   Quadrature Encoder Interface (QEI)
-   I2C multi-device communication
-   UART CLI implementation
-   Debouncing and input conditioning
-   Modular driver-based firmware structure

------------------------------------------------------------------------

## Potential Enhancements

-   PID-based closed-loop motor control
-   FreeRTOS integration
-   CAN or Ethernet communication support
-   Data logging to non-volatile memory
-   Bootloader implementation

------------------------------------------------------------------------

## Conclusion

This project demonstrates the design and implementation of a complete
real-time embedded control system integrating sensing, communication,
motor control, and user interaction. It reflects strong firmware
development fundamentals and practical embedded systems experience.
