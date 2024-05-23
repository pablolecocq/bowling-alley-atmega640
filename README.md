# Automatic Bowling Alley — ATmega640

Control of an **automatic bowling-alley model** using an **AVR ATmega640** microcontroller, programmed in C. The system manages the full game cycle: ball launch, ball return, lift, and pin scoring on 7-segment displays, all coordinated by a **state machine**.

Team project developed for the **Microprocessor Systems** course (4th year, Bachelor's in Industrial Technology Engineering — ETSII, UPM).

---

## System overview

The model is organized into three coordinated subsystems:

1. **Launcher** — ball movement, aiming and firing (motors M2, M3, M4) plus its lighting.
   - `home_lanzador()` — moves the launcher to the loading position.
   - `mov_vastago()` — tensions the rubber band and sets the retainer.
   - `giro_lanzador()` — oscillating aiming motion, waiting for the fire input.
   - `disparo()` — releases the retainer and launches the ball.
2. **Ball return and lift** — motors M1 and M5, driven by push-button SW6 (start game / launch).
3. **Scoreboard (displays)** — count of knocked-down pins on 7-segment displays.

---

## Technical details

- **MCU:** AVR ATmega640 (8 MHz).
- **5-motor control** through a single `control_motor()` function (nested switch): takes the motor, its enable, direction and brake signal (*brake* only used on M2).
- **PWM:** **8-bit Fast PWM**, prescaler 1024, comparison with `OCR1A`/`OCR1B` → ~30 Hz for the motors. Parameters defined in [`macros.h`](integrado/macros.h).
- **Interrupts** for sensors and limit switches:
  - `INT0` (SW1), `PCINT7` (SW5), `PCINT16` (SW6), plus external interrupts for SW2/SW3/SW6 (falling edge).
- **Software debouncing** on every push-button, plus an n-millisecond delay helper.
- **State machine** in `main.c`: `posicion0 → home → vastago → izquierda → cabeceo → subir_bola → disparo → ascensor → …`
- **Inter-subsystem communication** via flags read through `get_*()` functions (e.g. `get_derecha_M2()`, `get_vastago_atras()`), enabling modular integration.

> Source code and identifiers are in Spanish, as originally written for the course.

---

## Repository structure

```
bowling-alley-atmega640/
├── docs/
│   └── memoria.pdf          Technical report (Spanish)
├── integrado/               Final integrated version (all subsystems)
│   ├── main.c               Main state machine
│   ├── funciones_lanzador.c Motor and launcher control
│   ├── Retorno.c            Ball return and lift
│   ├── displays.c           Pin scoreboard (7-segment)
│   ├── Timer0.c             Time base
│   └── macros.h             Pin, motor and PWM parameter mapping
└── partes/                  Modular per-subsystem development (before integration)
    ├── disparador/          Launcher — two iterations
    │   ├── sin-pwm/         Early version, without PWM speed control
    │   └── con-pwm/         Later version, with PWM speed control
    ├── displays-bolos/
    └── retorno/
```

`integrado/` holds the final code that runs on the model; `partes/` keeps the standalone development of each subsystem, showing how it was built modularly before integration.

---

## Tech stack

`C` · `AVR ATmega640` · `avr-gcc` · `PWM` · `Interrupts` · `State machine` · `7-segment displays`

---

## Authors

Team project (Group 1) — Microprocessor Systems, Bachelor's in Industrial Technology Engineering, ETSII (UPM).

Lorena Rodríguez · Gabriela Artaza · Camino Luque · Javier Sanz · Óscar Flórez · Pablo Lecocq.
