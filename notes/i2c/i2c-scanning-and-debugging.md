# I2C: scanning and debugging the bus

## Always start with a scan

```cpp
for (uint8_t a = 1; a < 127; a++) {
  Wire.beginTransmission(a);
  if (Wire.endTransmission() == 0) Serial.printf(" 0x%02X", a);
}
```

Common addresses: SSD1306 OLED `0x3C` (sometimes `0x3D`), PCA9685 `0x40` (`0x70` is its all-call address).

## Lesson: don't trust `begin()` alone

The Adafruit SSD1306 library's `oled.begin()` returned **OK** while the display was not on the bus
at all. It mostly checks that it could allocate the screen buffer. **Trust the scan**, or ping the
address yourself and check the ACK.

## Debugging a missing device (in this order)

1. **Is another device on the same bus working?** If yes, the MCU pins are fine; the problem is the
   missing device's own wires.
2. **Power:** measure VCC to GND on the module's own pins (not the breadboard row).
3. **Pin order:** read the labels on the module. OLEDs come as `GND VCC SCL SDA` or `VCC GND SCL SDA`.
   SCL is sometimes labelled **SCK**.
4. **SDA/SCL swapped or doubled up:** a swap makes only that device disappear; the others keep working.
5. **Continuity, power off:** meter in beep mode from the module pin to the MCU pin itself.
6. **Unsoldered headers:** voltage can read fine while the data pins have no contact.
7. **Swap in a spare module** to separate "bad part" from "bad wiring".

## Keep checking while running

Ping each device a couple of times a second and count failures. A rising count during motor
movement points to noise or a power dip.

Used in: [esp32-servo-robot](../../projects/esp32-servo-robot/)
