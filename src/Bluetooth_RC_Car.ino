/*
  Bluetooth Controlled Car — Arduino UNO (ATmega328P)
  Nirma University | Electronics Circuits (2EC105CC25)
  Authors: Harshiul Manasara (24BEC139), Arjun Hirani (24BEC138)

  Hardware:
    - Arduino UNO R3 (ATmega328P)
    - HC-05 Bluetooth module  -> SoftwareSerial on D9 (RX), D10 (TX)
    - L293D / L298N motor driver -> 4x DC motor channels
    - 4x DC gear motors (3V-6V)
    - 2x 18650 Li-ion cells (~7.4V) as motor/board supply

  Requires the Adafruit Motor Shield library (AFMotor):
  https://learn.adafruit.com/adafruit-motor-shield/library-install

  Caution: disconnect the battery / remove the shield jumper before
  connecting the Arduino to a computer over USB.
*/

#include <AFMotor.h>
#include <SoftwareSerial.h>

SoftwareSerial bluetoothSerial(9, 10); // RX, TX

// Motor channel initialization (see docs/wiring.md for channel-to-pin mapping)
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

char command;

void setup() {
  bluetoothSerial.begin(9600); // match this to your HC-05 module's baud rate
}

void loop() {
  if (bluetoothSerial.available() > 0) {
    command = bluetoothSerial.read();

    Stop(); // reset to a known state before applying the next command

    switch (command) {
      case 'F': forward(); break;
      case 'B': back();    break;
      case 'L': left();    break;
      case 'R': right();   break;
      // any other byte falls through and the car stays stopped
    }
  }
}

void forward() {
  motor1.setSpeed(255); motor1.run(FORWARD);
  motor2.setSpeed(255); motor2.run(FORWARD);
  motor3.setSpeed(255); motor3.run(FORWARD);
  motor4.setSpeed(255); motor4.run(FORWARD);
}

void back() {
  motor1.setSpeed(255); motor1.run(BACKWARD);
  motor2.setSpeed(255); motor2.run(BACKWARD);
  motor3.setSpeed(255); motor3.run(BACKWARD);
  motor4.setSpeed(255); motor4.run(BACKWARD);
}

void left() {
  motor1.setSpeed(255); motor1.run(BACKWARD);
  motor2.setSpeed(255); motor2.run(BACKWARD);
  motor3.setSpeed(255); motor3.run(FORWARD);
  motor4.setSpeed(255); motor4.run(FORWARD);
}

void right() {
  motor1.setSpeed(255); motor1.run(FORWARD);
  motor2.setSpeed(255); motor2.run(FORWARD);
  motor3.setSpeed(255); motor3.run(BACKWARD);
  motor4.setSpeed(255); motor4.run(BACKWARD);
}

void Stop() {
  motor1.setSpeed(0); motor1.run(RELEASE);
  motor2.setSpeed(0); motor2.run(RELEASE);
  motor3.setSpeed(0); motor3.run(RELEASE);
  motor4.setSpeed(0); motor4.run(RELEASE);
}
