# 01 · ESP32 servo robot

ESP32 (38-pin DevKit) + PCA9685 servo driver + 0.96" SSD1306 OLED, driving MG90S servos.

## Current setup: 2 servos, single power bank

- Power bank 5 V 2 A → USB-C breakout → rocker switch → breadboard + rail (5 V).
- + rail → PCA9685 screw terminal V+ (servo power) and → ESP32 5V pin via jumper **J1**.
- − rail → PCA9685 screw terminal GND and ESP32 GND.
- ESP32 3V3 → PCA9685 VCC and OLED VCC.
- GPIO21 → SDA (PCA9685, OLED), GPIO22 → SCL/SCK (PCA9685, OLED).
- Servo 1 → PCA9685 ch0, Servo 2 → ch1 (brown wire on GND row).
- Optional caps: 1000 µF across the PCA9685 screw terminal, 470 µF across the rails (stripe to −).
- Leave unconnected: PCA9685 OE, left-header GND, both header V+ pins.

Worst case ≈ 1.75 A (2 servos at stall + logic) against the bank's 2 A.
Three or more servos need a separate servo battery — see `diagrams/esp32_servo_robot_wiring.html`.

### Safety rules

- **Pull J1 whenever the laptop USB is plugged into the ESP32.**
- OLED and PCA9685 VCC go to 3V3, never 5 V.
- Plug/unplug servos only with the switch OFF.
- Never power servos from the ESP32's 5V or 3V3 pins.

## Firmware (`firmware/`)

PlatformIO bring-up test: I2C scan, boot counter and reset reason on OLED + serial,
then repeating sweeps (servo 1, servo 2, both 60 ms apart). Serial commands:
`s` stop, `g` go, `r` rescan I2C.

```bash
cd firmware && ~/.platformio/penv/bin/pio run -t upload -t monitor
```

`platformio.ini` pins the port to `/dev/cu.usbserial-0001`; change it if your board enumerates differently.

## Diagrams (`diagrams/`)

- `esp32_2servo_wiring.html`: two-servo power-bank setup (point-to-point version).
- `esp32_servo_robot_wiring.html`: full 6-servo design with a separate 3S servo pack and two buck converters.

## Test result (2026-11-23)

I2C scan found 0x3C (OLED) and 0x40 (PCA9685). Over 2 full sweep cycles, including both servos
moving together: no resets, reset reason POWER-ON, 0 I2C errors.

## What went wrong

- **OLED not detected.** The I2C scan found only the PCA9685 (0x40). Power at the OLED read fine;
  the data wires were wrong. Fixed by wiring SCK → GPIO22 and SDA → GPIO21.
  The SSD1306 library's `begin()` reported OK the whole time, so trust the I2C scan instead.
- **Boot count jumping during testing** was not a fault: opening the serial port resets the board.
  The reset reason stayed POWER-ON, never BROWNOUT.

## Learnings

- [Servo power budgeting](../../notes/power/servo-power-budgeting.md)
- [I2C scanning and debugging](../../notes/i2c/i2c-scanning-and-debugging.md)
- [ESP32 pins to avoid](../../notes/esp32/pins-to-avoid.md)
- [Brownout and reset debugging](../../notes/esp32/brownout-debugging.md)

## Next steps

- Add the breadboard wiring diagram to `diagrams/`
- Add photos to `media/`
- Separate servo battery so it can grow to 6–10 servos
