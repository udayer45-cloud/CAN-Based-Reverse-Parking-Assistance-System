# CAN-Based Reverse Parking Assistance System

A two-node embedded system built on the **LPC2129 (ARM7TDMI-S)** microcontroller that replicates a real automotive reverse parking assist feature. Node A detects reverse gear engagement and drives a proximity buzzer. Node B measures obstacle distance using an HC-SR04 ultrasonic sensor and transmits data over a **CAN 2.0B bus at 125 Kbps**.

---

## Table of Contents

- [System Overview](#system-overview)
- [Hardware](#hardware)
- [Pin Configuration](#pin-configuration)
- [CAN Communication Protocol](#can-communication-protocol)
- [Firmware Architecture](#firmware-architecture)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Working Principle](#working-principle)
- [Known Limitations](#known-limitations)

---

## System Overview

```
  NODE A (Driver Side)               NODE B (Sensor Side)
  ┌─────────────────────┐            ┌─────────────────────┐
  │  LPC2129            │            │  LPC2129            │
  │                     │            │                     │
  │  SW (P0.16/EXT0) ──►│            │◄── HC-SR04 TRIG P0.3│
  │  Buzzer (P0.21)  ◄──│            │    HC-SR04 ECHO P0.4│
  │                     │            │                     │
  └──────────┬──────────┘            └──────────┬──────────┘
             │  CANH                             │
             ├───────────────────────────────────┤
             │  CANL                             │
           120Ω                               120Ω
        (termination)                     (termination)
```

- Node A sends an **RTR frame** when reverse gear is engaged
- Node B responds with **distance data** every 60 ms
- Node A drives the buzzer at a speed proportional to the measured distance
- Node B sends a stop response when reverse is disengaged

---

## Hardware

| Component | Details |
|---|---|
| Microcontroller | NXP LPC2129 (ARM7TDMI-S, 60 MHz) |
| CAN Transceiver | MCP2551 (on both nodes) |
| Ultrasonic Sensor | HC-SR04 (range: 2 cm – 400 cm) |
| Buzzer | Active buzzer, 5V |
| Reverse Switch | Normally open, active-LOW |
| Termination | 120Ω resistors at both ends of CAN bus |
| IDE | Keil µVision 5 |
| Compiler | ARM Compiler v5 |

---

## Pin Configuration

### Node A

| Pin | Direction | Function |
|---|---|---|
| P0.0 | Output | UART0 TX |
| P0.1 | Input | UART0 RX |
| P0.16 | Input | EXT0 — Reverse gear switch (active-LOW) |
| P0.21 | Output | Buzzer |
| P0.25 | Output | CAN2 TD2 |
| P0.26 | Input | CAN2 RD2 |

### Node B

| Pin | Direction | Function |
|---|---|---|
| P0.0 | Output | UART0 TX |
| P0.1 | Input | UART0 RX |
| P0.3 | Output | HC-SR04 TRIG |
| P0.4 | Input | HC-SR04 ECHO |
| P0.25 | Output | CAN2 TD2 |
| P0.26 | Input | CAN2 RD2 |

---

## CAN Communication Protocol

| Parameter | Value |
|---|---|
| CAN Standard | CAN 2.0B |
| Baud Rate | 125 Kbps |
| Frame ID | 0x501 |
| Frame Format | Standard (11-bit ID) |
| PCLK | 60 MHz (VPBDIV = 1) |
| C2BTR | 0x001C001D |

### Frame Types

**RTR Frame — Node A → Node B (Reverse Engaged)**
| Field | Value |
|---|---|
| ID | 0x501 |
| RTR | 1 |
| DLC | 0 |
| Data | None |

**Data Frame — Node B → Node A (Distance Response)**
| Field | Value |
|---|---|
| ID | 0x501 |
| RTR | 0 |
| DLC | 4 |
| Data | Distance in cm (byteA) |

**Stop Frame — Node A → Node B (Reverse Disengaged)**
| Field | Value |
|---|---|
| ID | 0x501 |
| RTR | 0 |
| DLC | 4 |
| Data | 0x1 (stop command) |

---

## Firmware Architecture

### Node A

| File | Description |
|---|---|
| `nodeA_main.c` | Main loop — handles gear switch flag, buzzer control |
| `can_driver_nodeA.c` | CAN2 init, TX function |
| `interrupt_nodeA.c` | CAN2 RX ISR, EXT0 ISR, VIC configuration |
| `uart0_driver.c` | UART0 init, TX, RX, string and integer print |
| `delay.c` | Blocking ms delay using Timer0 |
| `header.h` | Struct definition, typedefs, function declarations |

### Node B

| File | Description |
|---|---|
| `nodeB_main.c` | Main loop — handles CAN RX flag, triggers measurement |
| `can_driver_nodeB.c` | CAN2 init, TX function |
| `interrupt_nodeB.c` | CAN2 RX ISR, Timer0 ISR, VIC configuration |
| `hc_sr04.c` | HC-SR04 trigger, echo measurement, distance calculation |
| `delay_us.c` | Blocking µs delay using Timer1 |
| `uart0_driver.c` | UART0 init, TX, RX, string and integer print |
| `header.h` | Struct definition, typedefs, function declarations |

### Interrupt / Timer Usage

| Timer | Node | Purpose |
|---|---|---|
| Timer0 | Node A | Blocking ms delay (buzzer) |
| Timer0 | Node B | 60 ms measurement interval |
| Timer1 | Node B | Echo pulse width measurement + µs delay |

### VIC Slot Assignments

**Node A**
| Slot | Channel | ISR |
|---|---|---|
| 0 | 27 | CAN2 RX |
| 1 | 14 | EXT0 (gear switch) |

**Node B**
| Slot | Channel | ISR |
|---|---|---|
| 0 | 27 | CAN2 RX |
| 1 | 4 | Timer0 |

---

## Project Structure

```
can-reverse-parking-assistance-lpc2129/
├── NodeA/
│   ├── header.h
│   ├── nodeA_main.c
│   ├── can_driver_nodeA.c
│   ├── interrupt_nodeA.c
│   ├── uart0_driver.c
│   └── delay.c
├── NodeB/
│   ├── header.h
│   ├── nodeB_main.c
│   ├── can_driver_nodeB.c
│   ├── interrupt_nodeB.c
│   ├── uart0_driver.c
│   ├── hc_sr04.c
│   └── delay_us.c
└── README.md
```

---

## Build Instructions

1. Open Keil µVision 5
2. Create a new project, select **NXP LPC2129** as target device
3. Set compiler to **ARM Compiler v5**
4. Add all `.c` files from the respective node folder to the project
5. In Target Options → Target tab:
   - IRAM1: 0x40000000, size 0x4000
   - IROM1: 0x00000000, size 0x40000
6. In Target Options → C/C++ tab, add include path to the folder containing `header.h`
7. Build and flash using ULINK or J-Link debugger

> Flash Node B first, then Node A. Node B must be ready on the CAN bus before Node A starts sending RTR frames.

---

## Working Principle

1. Car starts → Node A initializes CAN, UART, EXT0 interrupt
2. Driver engages reverse gear → switch pulls P0.16 LOW → EXT0 ISR fires → `f2` flag set
3. Node A main loop detects `f2` → transmits **RTR frame** (ID 0x501) to Node B
4. Node B CAN RX ISR receives RTR → sets `f1` flag → main loop starts Timer0
5. Timer0 fires every 60 ms → Node B triggers HC-SR04 on P0.3 (10µs pulse)
6. HC-SR04 echo pulse on P0.4 → Timer1 measures pulse width → distance calculated
7. Node B transmits distance as **data frame** back to Node A
8. Node A CAN RX ISR receives distance → sets `f1` → main loop drives buzzer
9. Buzzer frequency increases as obstacle gets closer:

| Distance | Buzzer Delay |
|---|---|
| > 300 cm | 1000 ms |
| 200 – 300 cm | 500 ms |
| 100 – 200 cm | 250 ms |
| < 100 cm | 100 ms |

10. Driver disengages reverse → Node A sends **stop data frame** → Node B stops Timer0

---

## Known Limitations

- HC-SR04 returns 999 cm on timeout (no obstacle detected or sensor disconnected) — Node A treats this as out-of-range and maintains slowest buzzer rate
- No AUTOSAR stack — bare-metal implementation for learning purposes
- Single CAN ID used (0x501) — not suitable for multi-node extension without ID redesign
- No error frame handling — TEC/REC counters not monitored

---

## Author

**Chalampalem Udaykiran**  
Embedded Systems Trainee — Vector India Pvt. Ltd., Bengaluru  
B.Tech EEE — JNTUA College of Engineering, Kalikiri  
GitHub: [udayer45-cloud](https://github.com/udayer45-cloud)  
LinkedIn: [udaykiran1807](https://www.linkedin.com/in/udaykiran1807)
