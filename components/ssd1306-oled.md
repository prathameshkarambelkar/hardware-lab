# SSD1306 0.96" OLED (128×64, I2C)

| | |
|---|---|
| Interface | I2C, address `0x3C` (some modules `0x3D`) |
| VCC | 3.3 V from the ESP32 works; many modules also accept 5 V, but check yours |
| Pins | 4-pin; order varies: `GND VCC SCK SDA` on mine. SCK = SCL |
| Library | [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) + Adafruit GFX |

## Gotchas

- **Read the pin labels**; GND and VCC are swapped between brands.
- `oled.begin()` can return true even if the display is missing. Confirm with an I2C scan.
- Mine wasn't detected until SCK was correctly wired to GPIO22 and SDA to GPIO21; see
  [I2C debugging](../notes/i2c/i2c-scanning-and-debugging.md).
- Call `oled.display()` after drawing; nothing appears until you do.
