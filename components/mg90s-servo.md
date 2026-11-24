# MG90S metal-gear micro servo

| | |
|---|---|
| Voltage | 4.8–6 V |
| Current | ~10 mA idle, ~200 mA moving light load, ~700 mA stall |
| Signal | 50 Hz PWM, ~1000–2000 µs pulse (wider ranges hit the end stops) |
| Wires | brown = GND, red = V+, orange = signal |

## Gotchas

- Every move starts with a short stall-level surge. Budget for stall current: see
  [servo power budgeting](../notes/power/servo-power-budgeting.md).
- Holding the horn by hand stalls it at full current.
- Plug or unplug only with servo power off.
- Staying at 1000–2000 µs keeps clear of the mechanical end stops; push the range only after testing.
