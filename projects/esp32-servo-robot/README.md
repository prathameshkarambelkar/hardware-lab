# 01 · ESP32 servo robot

## What it does

An ESP32 drives two MG90S servos through a PCA9685 and shows its status on an OLED. Everything
runs from one 5 V power bank. This is the first step towards a 6–10 servo robot.

## Components needed

| Component | Qty | Notes |
|---|---|---|
| ESP32-WROOM-32 DevKit, 38-pin | 1 | |
| PCA9685 16-channel PWM servo driver | 1 | address 0x40 |
| SSD1306 0.96″ OLED, 128×64, I2C | 1 | pin order on mine: `GND VCC SCK SDA` |
| MG90S metal-gear micro servo | 2 | ~700 mA each at stall |
| Power bank, 5 V 2 A, USB-C | 1 | |
| USB-C breakout (female) | 1 | |
| SPST rocker switch | 1 | |
| Half-size breadboard (400 point) | 1 | use one rail pair only |
| Male-male jumper wires | ~15 | |
| Electrolytic capacitors, 1000 µF and 470 µF, 16 V | 1 each | optional |

**Tools needed:** multimeter, laptop with PlatformIO, USB cable for the ESP32.

## Power

- **Supply:** 5 V 2 A power bank = 10 W.
- **Rails:**

| Rail | Voltage | Feeds | Worst-case current |
|---|---|---|---|
| Breadboard + / − rail | 5 V | PCA9685 screw terminal (servos), ESP32 5V pin via J1 | 1.75 A |
| ESP32 3V3 | 3.3 V | OLED, PCA9685 logic | ~0.1 A |

- **Why it's safe:** 2 servos at stall (1.4 A) + logic (0.35 A) = 1.75 A, under the bank's 2 A.
  Three or more servos are **not** safe on this supply; see `diagrams/esp32_servo_robot_wiring.html`
  for the 6-servo design with a separate battery.

## Wiring diagram

Open [diagrams/esp32_2servo_wiring.html](diagrams/esp32_2servo_wiring.html) (prints on A4 landscape).
On a breadboard, the "split" point is the breadboard + / − rail.

| # | From | To | Notes |
|---|---|---|---|
| 1 | USB-C breakout VBUS | switch pin 1 | |
| 2 | switch pin 2 | breadboard + rail | rail now carries 5 V |
| 3 | USB-C breakout GND | breadboard − rail | |
| 4 | + rail | PCA9685 screw terminal V+ | 2 jumpers side by side |
| 5 | − rail | PCA9685 screw terminal GND | 2 jumpers side by side |
| 6 | + rail | ESP32 5V | **J1**, pull when laptop USB is in |
| 7 | − rail | ESP32 GND | |
| 8 | ESP32 3V3 | PCA9685 VCC and OLED VCC | |
| 9 | ESP32 GND | OLED GND | |
| 10 | ESP32 GPIO21 | PCA9685 SDA and OLED SDA | |
| 11 | ESP32 GPIO22 | PCA9685 SCL and OLED SCK | |
| 12 | Servo 1 / Servo 2 | PCA9685 ch0 / ch1 | brown wire on the GND row |
| 13 | 1000 µF | across PCA9685 screw terminal | optional, stripe to GND |
| 14 | 470 µF | across + / − rail | optional, stripe to − |

**Leave unconnected:** PCA9685 OE, PCA9685 side-header GND, both PCA9685 side-header V+ pins.

⚠ **Can damage parts:**
- **OLED VCC and PCA9685 VCC go to 3V3, never 5 V.**
- **Pull J1 before plugging the laptop into the ESP32**, or two 5 V supplies meet.
- **Never power servos from the ESP32 5V or 3V3 pins.**
- **Capacitor stripe = negative → GND.** Reversed, it can burst.
- **Plug or unplug servos only with the switch OFF.**

## Build steps

1. **Do:** connect the power bank to the USB-C breakout, switch ON, nothing else wired.
   ✓ **Check:** USB-C breakout VBUS to GND: 4.9–5.2 V. **If 0 V:** press the power bank's button;
   if still 0 V, the bank isn't detecting a load.
2. **Do:** switch OFF. Wire 2–7 (rails, PCA9685 power, ESP32 power).
   ✓ **Check:** continuity mode, ESP32 5V to ESP32 3V3: no beep.
3. **Do:** wire 8–11 (logic and I2C). Pull J1, plug in the laptop, flash the firmware (see Code).
   ✓ **Check:** serial output shows I2C scan `0x3C 0x40`. **If 0x3C is missing:** see
   [I2C debugging](../../notes/i2c/i2c-scanning-and-debugging.md).
4. **Do:** switch OFF, plug in Servo 1 and Servo 2, switch ON.
   ✓ **Check:** servos sweep; OLED keeps updating; + rail to − rail stays ≥ 4.6 V while both move.
   **If the ESP32 resets:** check that no servo power reaches the ESP32 through a second path.

## Code

```bash
cd firmware && ~/.platformio/penv/bin/pio run -t upload -t monitor
```

- Scans I2C and shows a boot counter and the last reset reason on the OLED and serial.
- Waits 10 s ("Switch ON now"), then loops: Servo 1 sweeps, Servo 2 sweeps, both together (60 ms apart).
- Pings the OLED and PCA9685 twice a second and counts failures.
- Serial commands: `s` stop, `g` go, `r` rescan I2C.
- `platformio.ini` sets the port to `/dev/cu.usbserial-0001`; change it if yours differs.

## Test result (2026-11-23)

| Check | Expected | Measured |
|---|---|---|
| I2C scan | 0x3C, 0x40 | 0x3C, 0x40 ✓ |
| Reset reason | POWER-ON / EN, never BROWNOUT | POWER-ON ✓ |
| Boot counter during 2 full cycles | no change | no change ✓ |
| I2C errors (OLED / PCA9685) | 0 / 0 | 0 / 0 ✓ |

## What went wrong

- **Symptom:** OLED blank; I2C scan found only 0x40. **Cause:** OLED data wires wrong.
  **How I found it:** PCA9685 on the same pins worked, and OLED VCC read 3.3 V, so it had to be its
  SCK/SDA wires. **Fix:** OLED SCK → ESP32 GPIO22, OLED SDA → ESP32 GPIO21.
  The SSD1306 library's `begin()` reported OK the whole time, so trust the I2C scan.
- **Symptom:** boot counter went up during testing. **Cause:** not a fault; opening the serial port
  resets the ESP32. Reset reason stayed POWER-ON.

## Learnings

- [Servo power budgeting](../../notes/power/servo-power-budgeting.md)
- [I2C scanning and debugging](../../notes/i2c/i2c-scanning-and-debugging.md)
- [ESP32 pins to avoid](../../notes/esp32/pins-to-avoid.md)
- [Brownout and reset debugging](../../notes/esp32/brownout-debugging.md)

## Next steps

- Add the breadboard version of the wiring diagram to `diagrams/`
- Add photos to `media/`
- Separate servo battery (3S Li-ion + 2 bucks) to grow to 6–10 servos
