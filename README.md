# hardware-lab

My lab notebook for robotics and embedded hardware: things I build, what I learn along the way,
and the resources I keep coming back to. Everything is written from real bench work, including
what went wrong.

## Projects

| # | Project | What it is | Status |
|---|---------|-----------|--------|
| 01 | [esp32-servo-robot](projects/esp32-servo-robot/) | ESP32 + PCA9685 + OLED driving MG90S servos | 2 servos working on a power bank |

## Notes (learnings by topic)

- **Power**: [Servo power budgeting](notes/power/servo-power-budgeting.md)
- **ESP32**: [Pins to avoid](notes/esp32/pins-to-avoid.md) · [Brownout and reset debugging](notes/esp32/brownout-debugging.md)
- **I2C**: [Scanning and debugging the bus](notes/i2c/i2c-scanning-and-debugging.md)
- **Motors**: _coming soon_
- **Tools**: _coming soon_

## Components

Specs, pinouts and gotchas for parts I've used:
[ESP32 DevKit](components/esp32-devkit.md) ·
[PCA9685](components/pca9685.md) ·
[SSD1306 OLED](components/ssd1306-oled.md) ·
[MG90S servo](components/mg90s-servo.md)

## Resources

- [Links](resources/links.md): datasheets, docs and libraries
- [Inventory](resources/inventory.md): parts on my bench

## Layout

```
projects/     things I built, one folder each (firmware, diagrams, media, writeup)
notes/        learnings by topic, reusable across projects
components/   one page per part: specs, pinout, gotchas
resources/    links and inventory
templates/    starting point for new project writeups
```

## A word of caution

Wiring and power designs here worked on my bench with my parts. Check voltages and current ratings
against your own parts before copying anything. Wrong wiring can destroy components, and lithium
batteries can catch fire.

## Contributing / AI assistants

Writing, structure and diagram rules are in [AGENTS.md](AGENTS.md). New projects start from
[templates/project-readme.md](templates/project-readme.md).

## License

Code is MIT. Notes, diagrams and images are CC BY 4.0. See [LICENSE](LICENSE).
