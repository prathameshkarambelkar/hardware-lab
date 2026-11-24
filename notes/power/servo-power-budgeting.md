# Servo power budgeting

How to work out whether a power source can run your servos without the microcontroller resetting.

## Use stall current, not running current

A hobby servo draws its stall current briefly at the start of **every** move, not only when it's
jammed. Budget for stall.

| MG90S (typical) | Per servo |
|---|---|
| Idle, powered | ~10 mA |
| Moving, light load | ~200 mA |
| Stall / start of a move | ~700 mA |

So 6 servos = 4.2 A (21 W at 5 V) worst case. A 5 V 2 A power bank is only 10 W.

## Rules of thumb

- **Total worst case = servos at stall + logic.** Keep it under the supply's rating.
- **Converters can't add power.** Boosting then bucking just loses some. If the source is 10 W, you have < 10 W.
- **Never parallel two buck outputs.** They fight; one ends up carrying the whole load. Give each its own group of servos instead.
- **Check the buck's minimum input.** A "5 V 3 A" buck that needs 7 V in is useless on a 5 V bank.
- **Wires matter.** Dupont jumpers are fine for ~1–2 A; above that use thicker wire (20 AWG) and screw terminals.
- **Keep servo current out of the MCU board.** Servo power goes straight to the servo driver's power terminal, never through the ESP32's pins.

## Worked example: 2 servos on a 5 V 2 A bank

| Load | Worst case |
|---|---|
| 2 × MG90S at stall | 1.4 A |
| ESP32 (WiFi on) + OLED + PCA9685 logic | 0.35 A |
| **Total** | **1.75 A** (under 2 A) |

Breadboard rail resistance ≈ 0.05 Ω → 0.07 V drop at 1.4 A. The ESP32 still sees ~4.6 V, and its
regulator needs ~4.3 V to hold 3.3 V. Safe. Three or more servos: not safe on this supply.

## Cheap tricks that buy headroom

- Stagger servo starts by 20–60 ms so the stall surges don't line up.
- Ramp movements in small steps instead of jumping straight to the target.
- 470–1000 µF electrolytic across the servo power terminal (stripe to GND) covers short surges.

Used in: [esp32-servo-robot](../../projects/esp32-servo-robot/)
