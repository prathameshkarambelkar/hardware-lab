# ESP32 38-pin DevKit (ESP32-WROOM-32)

| | |
|---|---|
| Logic | 3.3 V; on-board regulator makes 3V3 from the 5V pin / USB |
| 5V pin | Input from USB or an external 5 V supply; don't feed both at once |
| 3V3 pin | Fine for small loads (OLED, sensor logic, PCA9685 logic) |
| USB-serial | Shows up as `/dev/cu.usbserial-*` on macOS (CP210x / CH340) |
| Toolchain | PlatformIO, `board = esp32dev`, Arduino framework |

## Gotchas

- **Never connect an external 5 V supply to the 5V pin while USB is plugged in.** The two supplies
  meet. Pull that jumper first.
- If an upload hangs at `Connecting....`, hold BOOT until it starts.
- Opening the serial monitor resets the board.
- Pin choices: see [pins to avoid](../notes/esp32/pins-to-avoid.md).
- Reset debugging: see [brownout debugging](../notes/esp32/brownout-debugging.md).

Datasheet: [Espressif ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
