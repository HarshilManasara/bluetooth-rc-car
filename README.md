# Bluetooth Controlled Car

A wirelessly controlled 4-motor robotic car built on an Arduino UNO, driven over Bluetooth from a smartphone. Built as a project for the Electronics Circuits (2EC105CC25) course at Nirma University's Institute of Technology.

![Platform](https://img.shields.io/badge/platform-Arduino%20UNO-00979D?logo=arduino&logoColor=white)
![Language](https://img.shields.io/badge/language-C%2B%2B-blue)
![Comm](https://img.shields.io/badge/link-Bluetooth%20HC--05-0082FC)
![License](https://img.shields.io/badge/license-MIT-green)

**Authors:** Arjun Hirani ([@ArjunHirani](https://github.com/ArjunHirani)), Harshiul Manasara — B.Tech ECE, Nirma University (2024–2028)

---

## Demo

🎥 **[Watch the car in action](PASTE_YOUR_YOUTUBE_OR_DRIVE_LINK_HERE)** — live demonstration of forward/backward/left/right control over Bluetooth.

## Overview

A smartphone sends single-character movement commands (`F`, `B`, `L`, `R`) over Bluetooth to an HC-05 module wired to an Arduino UNO. The Arduino decodes each command and drives a 4-motor differential H-bridge configuration through an L293D/L298N motor driver, so the car moves forward, backward, or turns by running the left and right motor pairs in opposite directions.

```
 Mobile App (BT)  --UART-->  HC-05  --Serial-->  Arduino UNO  --Digital+PWM-->  L293D/L298N  -->  4x DC Motors
```

## Why this project

Most Bluetooth-car tutorials stop at "upload the code and drive it." This repo also documents the *why* behind the hardware layer — specifically how the motor driver's internal BJT H-bridge actually switches motor current — since that's the part most write-ups skip and the part that's actually interesting from an electronics standpoint. See [`docs/analog-deep-dive.md`](docs/analog-deep-dive.md).

## Hardware

| Ref | Component | Qty | Role |
|---|---|---|---|
| U1 | Arduino UNO R3 (ATmega328P, 16 MHz) | 1 | Reads Bluetooth commands, generates motor control signals |
| U2 | L293D / L298N motor driver module | 1 | H-bridge driver — converts Arduino logic signals into motor-drive current |
| U3 | HC-05 Bluetooth module | 1 | UART-over-Bluetooth link to the mobile app |
| M1–M4 | DC gear motors (3V–6V) | 4 | Wheel actuation |
| — | Car chassis + wheels | 1 | Mechanical platform |
| — | 18650 Li-ion cells | 2 | ~7.4V motor/board supply |
| — | Battery holder | 1 | Power connection |

Full pin mapping: [`docs/wiring.md`](docs/wiring.md) · Arduino pinout reference: [`docs/images/arduino-pinout.png`](docs/images/arduino-pinout.png)

## Firmware

Source: [`src/Bluetooth_RC_Car.ino`](src/Bluetooth_RC_Car.ino)

- `SoftwareSerial` on D9/D10 talks to the HC-05 independently of the USB serial line, so the board can be debugged over USB while Bluetooth is connected.
- Each incoming byte is matched against `F`/`B`/`L`/`R` in a `switch`; anything else (including `S`) leaves the motors stopped.
- Turning is done differentially: for a left turn, the left-side motors reverse while the right-side motors run forward, so the car pivots rather than relying on steering geometry.
- Motors are explicitly set to `RELEASE` between commands rather than left running, to avoid overlapping drive states.

### Dependencies
- [Adafruit Motor Shield library (AFMotor)](https://learn.adafruit.com/adafruit-motor-shield/library-install)
- Arduino IDE 1.8+ or Arduino CLI

### Flashing
1. Install the AFMotor library via **Sketch → Include Library → Manage Libraries**.
2. Wire the hardware per `docs/wiring.md`.
3. **Disconnect the battery / remove the power jumper before connecting USB** — the motor driver and USB supply should never be live at the same time.
4. Open `src/Bluetooth_RC_Car.ino`, select **Arduino UNO** as the board, and upload.
5. Pair the HC-05 with a Bluetooth serial-controller app (e.g. "Arduino Bluetooth Controller") and send `F` / `B` / `L` / `R`.

## Results

- The car responds correctly to all four commands with no noticeable lag under normal conditions.
- Bluetooth link is stable to roughly 5–10 m line-of-sight, consistent with the HC-05's rated Class 2 range.
- Motor response is directly affected by battery state of charge — as the 18650 pair discharges, all four motors slow proportionally, since the system runs the motors open-loop with no speed feedback.

## Limitations & Future Work

This is an open-loop system: there's no sensing of actual wheel speed, heading, or obstacles, so the car executes commands blindly. Planned/possible extensions:
- Ultrasonic or IR obstacle detection with an auto-stop safety cutoff (currently, a lost Bluetooth link means the car keeps executing its last command indefinitely — a real gap for anything beyond a demo)
- PWM-based analog speed control from the app instead of fixed full-speed driving
- Current sensing on the motor driver output to detect stall conditions
- Encoder feedback for closed-loop speed/heading control

## Repository Structure

```
bluetooth-rc-car/
├── src/
│   └── Bluetooth_RC_Car.ino      # Arduino firmware
├── docs/
│   ├── wiring.md                 # Pin mapping and connection diagram
│   ├── analog-deep-dive.md       # How the H-bridge driver actually works
│   └── images/
│       └── arduino-pinout.png
├── LICENSE
└── README.md
```

## License

MIT — see [LICENSE](LICENSE).
