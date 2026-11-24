# ESP32 pins to avoid (classic ESP32 / WROOM-32)

| Pins | Why to avoid |
|---|---|
| GPIO6–11 | Wired to the internal flash chip. Using them crashes the board. |
| **GPIO12** | Sets flash voltage at reset. A pull-up on it (e.g. from a module) stops the board booting. The most dangerous one. |
| GPIO0, 2, 5, 15 | Boot strapping pins. GPIO5 and GPIO15 also output PWM during boot. |
| GPIO1, GPIO3 | UART0: uploads and `Serial`. |
| GPIO34–39 | Input only, no internal pull-ups. Fine for analog reads or inputs with external pull-ups. |
| GPIO14 | Outputs PWM at boot, so connected loads may twitch. |
| GPIO16, 17 | Used for PSRAM on WROVER modules. |
| ADC2 pins | ADC2 stops working while WiFi is on. Use ADC1 (GPIO32–39) for analog. Digital use is fine. |

## Safe defaults I use

| Use | Pins |
|---|---|
| I2C | GPIO21 (SDA), GPIO22 (SCL) |
| General digital I/O | GPIO4, 13, 18, 19, 23, 25, 26, 27, 32, 33 |
| Analog with WiFi on | GPIO32–39 |

Reference: [Espressif GPIO docs](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html)
