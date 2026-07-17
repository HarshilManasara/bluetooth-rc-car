# How the Motor Driver Actually Works: Inside the H-Bridge

The firmware treats `motor.run(FORWARD)` as a black box. This note looks at what's actually happening electrically inside the L293D/L298N driver, since that's the one genuinely analog/power-electronics piece of this project — everything else is digital logic.

## Why you can't drive a DC motor from an Arduino pin directly

An ATmega328P GPIO pin can source/sink only a few tens of mA and swings between 0V and 5V. A small DC gear motor at stall can draw several hundred mA to over an amp, and reversing its direction means reversing the polarity across it — something a single GPIO pin physically cannot do (it can only ever be high or low relative to ground, never "positive on one terminal and negative on the other" the way a motor needs for reversal). You need a stage in between that can (a) supply real current from a separate power rail and (b) flip polarity on command from a low-power logic signal. That's the H-bridge's job.

## The H-bridge topology

Four switches arranged in an "H" around the motor:

```
        +VMOT
       /      \
     [S1]    [S3]
      |        |
      +--Motor-+
      |        |
     [S2]    [S4]
       \      /
        GND
```

- S1+S4 closed, S2+S3 open → current flows one way through the motor → forward
- S2+S3 closed, S1+S4 open → current flows the other way → reverse
- All open → motor coasts (freewheels)
- S1+S3 (or S2+S4) closed → both motor terminals shorted to the same rail → **brake** (this is the `RELEASE` state's electrical opposite; `RELEASE` in AFMotor actually means "all switches open," i.e. coast, not brake — worth knowing since a genuinely fast stop needs the short-both-low configuration instead)

In the L293D/L298N, those four switches are **Darlington-pair BJTs**, not MOSFETs — which is exactly why these older drivers have a comparatively high voltage drop (~1.4–2V) across the bridge and dissipate more heat than a modern MOSFET-based driver at the same current. That saturation voltage loss is a real, measurable design tradeoff: it's why the L298N gets warm on this project and why higher-current modern motor drivers (e.g. TI's DRV8xxx family) use MOSFET H-bridges instead — lower RDS(on), less heat, higher efficiency at the same current.

## Flyback diodes

A DC motor's windings are inductive. The instant a switch opens (e.g. going from forward to `RELEASE`), the inductor's current can't stop instantly — it has to go somewhere, and without a path it generates a large reverse voltage spike (back-EMF) that can exceed the transistor's breakdown voltage and destroy it. The L293D/L298N has flyback (freewheeling) diodes built into the driver IC across each switch specifically to give that inductive current a safe path to dissipate. This is the same principle used to protect a transistor driving a relay coil, and it's a standard analog-design question: "why do you need a diode across an inductive load?"

## Where this project's PWM actually sits

The firmware currently drives motors at a fixed `setSpeed(255)` (full duty cycle) — no speed variation. But the hardware path already supports PWM speed control, since the driver's enable pins accept a PWM signal whose duty cycle sets the average voltage (and therefore average current/torque) delivered to the motor, well below VMOT. Adding `setSpeed(n)` with variable `n` is a one-line firmware change that turns this from an on/off system into one with continuous (well, discretized via PWM) speed control — worth doing if you want to talk about PWM-based analog control in an interview rather than just switching logic.

## Talking points this unlocks

- Why a GPIO pin can't drive a motor directly, and what a driver stage needs to provide
- BJT-based vs MOSFET-based H-bridges — the RDS(on)/saturation-voltage tradeoff and why it matters for efficiency and heat
- Why inductive loads need flyback protection, and where in the driver IC that protection lives
- The difference between "coast" (open bridge) and "brake" (shorted bridge) stop states, and why this firmware only implements the former
- How PWM duty cycle on an H-bridge's enable line translates to average motor voltage/speed
