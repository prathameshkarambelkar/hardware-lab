# Brownout and reset debugging on the ESP32

A brownout reset happens when the ESP32's supply dips too low for a moment, typically when motors
start. It often looks like "the board randomly restarts".

## Make resets visible

1. **Print the reset reason** at boot with `esp_reset_reason()`. `ESP_RST_BROWNOUT` is the giveaway.
2. **Keep a boot counter** in flash (`Preferences`) and show it on a display. If the number jumps,
   the board reset even if you didn't notice.
3. **Show uptime.** If it drops back to 0, the board reset.

```cpp
#include <Preferences.h>
#include "esp_system.h"

Preferences prefs;
void setup() {
  esp_reset_reason_t rr = esp_reset_reason();
  prefs.begin("bringup", false);
  uint32_t boots = prefs.getUInt("boots", 0) + 1;
  prefs.putUInt("boots", boots);
  Serial.printf("Boot #%lu, brownout=%d\n", (unsigned long)boots, rr == ESP_RST_BROWNOUT);
}
```

## Not every reset is a fault

- **Opening the serial monitor resets the board** on most DevKits (DTR/RTS lines). The boot count
  goes up, and the reason shows as POWER-ON or EN, not BROWNOUT.
- **Uploading firmware** resets it too.

## Usual causes of real brownouts

- Servo or motor current flowing through the ESP32 board or a shared thin wire.
- A supply that can't handle the surge (check the [power budget](../power/servo-power-budgeting.md)).
- A second ground or power path that sends motor current through the logic side.
- Loose breadboard contacts on the power path.

## Fixes

- Separate the motor power path from the logic power path; share only ground.
- Add 470 µF across the ESP32's 5V and GND.
- Stagger motor starts.
