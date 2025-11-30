# Obstacle-Avoiding-Robot-w-STM32
Autonomous obstacle avoiding robot car using STM32F072B-DISCO, HC-SR04, L298N and HC-06.
# Obstacle-Avoiding Smart Robot Car (STM32F072B-DISCO)

This project implements an autonomous obstacle-avoiding robot car using the STM32F072B-DISCO board.  
The robot measures distance with an HC-SR04 ultrasonic sensor, drives two DC motors through an L298N motor driver, adapts its speed using an LDR connected to the ADC, and sends real-time debug data wirelessly via an HC-06 Bluetooth module.  
All peripherals are programmed at **register level** following the CMSIS standard.

---

## 1. Project Overview

In indoor environments (malls, factories, warehouses), manual control of mobile robots is inefficient and error-prone.  
This project aims to build a fully autonomous robot that:

- Detects obstacles in real time using the **HC-SR04**  
- Calculates distance using **Timer Input Capture**  
- Controls motor direction with **GPIO** and motor speed with **PWM**  
- Adapts motor speed according to ambient light using an **LDR + ADC**  
- Sends distance, speed, and state information wirelessly through **HC-06 Bluetooth (UART)**  

The software is written in C at register level without using the HAL driver.

---

## 2. Hardware Components

| Component                     | Function                                  |
|------------------------------|-------------------------------------------|
| STM32F072B-DISCO             | Main MCU board (Cortex-M0, 72 MHz)       |
| HC-SR04 Ultrasonic Sensor    | Distance measurement                      |
| L298N Motor Driver           | Dual H-bridge driver for 2 DC motors     |
| REX Robot Chassis + Motors   | Mechanical platform and DC motors        |
| 5 mm LDR + Resistor          | Ambient light sensor (ADC input)         |
| HC-06 Bluetooth Module       | Wireless UART debug link                 |
| LM2596 Buck Converter        | Step-down regulator (battery → 5 V)      |
| Battery Pack (7.4–9 V)       | Main power source                        |

---

## 3. Pin Connections (Logical)

| Device        | Signal           | STM32 Pin | Function / Interface        |
|--------------|------------------|----------|-----------------------------|
| HC-SR04      | TRIG             | PA1      | GPIO Output                 |
| HC-SR04      | ECHO             | PA2      | TIM2 Input Capture          |
| L298N        | IN1              | PB0      | GPIO Output (motor dir)     |
| L298N        | IN2              | PB1      | GPIO Output (motor dir)     |
| L298N        | ENA              | PA8      | TIM1_CH1 PWM Output         |
| LDR Sensor   | Divider Output   | PA4      | ADC_IN4                     |
| HC-06        | RX               | PA9      | USART1_TX (MCU → HC-06)     |
| HC-06        | TX               | PA10     | USART1_RX (HC-06 → MCU)     |
| Power        | 5 V              | 5V pin   | From LM2596 buck converter  |
| Power        | GND              | GND      | Common ground for all parts |

All grounds (STM32 board, L298N, HC-SR04, HC-06, LM2596, battery) are connected together.

---

## 4. System Line Diagram

```mermaid
graph TD
  BAT["Battery Pack 7-9V"]
  PSU["LM2596 Buck Converter"]
  US["HC-SR04 Ultrasonic Sensor"]
  ADC["LDR + Resistor (ADC Input)"]
  BT["HC-06 Bluetooth Module"]
  MCU["STM32F072B-DISCO (MCU)"]
  MD["L298N Motor Driver"]
  LM["Left DC Motor"]
  RM["Right DC Motor"]

  BAT --> PSU

  PSU -->|"5V Power"| MCU
  PSU -->|"5V Power"| US
  PSU -->|"5V Power"| BT
  PSU -->|"5V Power"| MD

  US -->|"TRIG PA1 GPIO OUT"| MCU
  US -->|"ECHO PA2 TIM2 INPUT CAPTURE"| MCU

  ADC -->|"PA4 ADC_IN4"| MCU

  BT -->|"TX HC06 TO PA10 UART RX"| MCU
  MCU -->|"PA9 UART TX TO HC06 RX"| BT

  MCU -->|"IN1 PB0 GPIO OUT"| MD
  MCU -->|"IN2 PB1 GPIO OUT"| MD
  MCU -->|"ENA PA8 TIM1 PWM"| MD

  MD -->|"Motor A"| LM
  MD -->|"Motor B"| RM
