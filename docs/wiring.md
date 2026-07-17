# Wiring & Pin Mapping

## Arduino UNO pin assignments

| Arduino Pin | Function | Mode | Connects to |
|---|---|---|---|
| D1 (TX) | UART TX | Serial output | HC-05 RX (via SoftwareSerial, D10 in firmware) |
| D6 (RX) | UART RX | Serial input | HC-05 TX (via SoftwareSerial, D9 in firmware) |
| D2 | IN1 (Motor A) | Digital out | Motor driver — Motor A direction |
| D3 | IN2 (Motor A) | Digital out | Motor driver — Motor A direction |
| D4 | IN3 (Motor B) | Digital out | Motor driver — Motor B direction |
| D5 | IN4 (Motor B) | Digital out | Motor driver — Motor B direction |

> Note: the AFMotor library manages the exact shield pin mapping internally when using `AF_DCMotor`. The table above reflects the logical signal path (Arduino → driver inputs), not necessarily literal Arduino digital pin numbers if you're using a stacked motor shield rather than a breakout module — check your specific shield's schematic if wiring by hand.

## Power

- Motors and driver: 2× 18650 Li-ion cells in series (~7.4V nominal) into the motor driver's VMOT input.
- Arduino: powered via USB during development, or via the driver module's onboard 5V regulator (if present) / a separate battery during standalone operation.
- **Never share a ground reference incorrectly** — the motor driver GND, battery GND, and Arduino GND must all be tied together for the logic signals to be referenced correctly.

## Bluetooth module

- HC-05 VCC → 5V (or 3.3V depending on your specific breakout — check before powering; some HC-05 breakouts have an onboard regulator, some don't)
- HC-05 GND → common ground
- HC-05 TX → Arduino D9 (SoftwareSerial RX)
- HC-05 RX → Arduino D10 (SoftwareSerial TX) — **through a voltage divider or logic-level shifter** if your module's RX is not 5V-tolerant, since the Arduino's TX swings to 5V and the HC-05 RX is typically a 3.3V input.

## Reference diagram

See [`images/arduino-pinout.png`](images/arduino-pinout.png) for the annotated Arduino UNO pin diagram.
