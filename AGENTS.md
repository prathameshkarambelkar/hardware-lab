# AGENTS.md: how to write in this repo

Rules for any AI assistant (Claude, ChatGPT, Gemini, Copilot, Cursor, …) or person adding to this
repo. Follow them so every project looks and reads the same.

**Audience:** a beginner with the parts on their desk. If they can't follow a step without asking
someone, rewrite it.

---

## 1. Writing style

- **Simple words, short sentences.** One idea per sentence. No jargon without a one-line explanation.
- **Tell the reader what to do**: "Connect ESP32 GPIO21 to OLED SDA." Not "SDA should be routed to…".
- **Tables for facts** (parts, pins, readings). **Numbered lists for steps.** Paragraphs only for "why".
- **Every number has a unit**: 5 V, 700 mA, 470 µF, 20 ms.
- **Every measurement says between which two points** and the expected reading:
  "Meter on DC volts, + rail to − rail: 4.9–5.2 V."
- **Every check says what to do if it's wrong.**
- **Warnings that can destroy parts** are marked ⚠ and written in bold.
- No marketing words, no emojis except ⚠ and ✓/✕ in tables.
- Keep it short. If a section passes one screen, split it or move the detail to `notes/`.

## 2. Component names

Write the full, correct name the first time, then the short name. Use the **same** short name
everywhere: README, diagram, code comments.

| Full name (first mention) | Short name |
|---|---|
| ESP32-WROOM-32 DevKit, 38-pin | ESP32 |
| PCA9685 16-channel PWM servo driver | PCA9685 |
| SSD1306 0.96″ OLED, 128×64, I2C | OLED |
| MG90S metal-gear micro servo | MG90S / Servo 1, Servo 2 |
| USB-C breakout (female) | USB-C breakout |
| SPST rocker switch | switch |
| MP2338 5 V 3 A buck converter | buck |
| MT3608 boost converter | boost |
| HW-040 rotary encoder | encoder |

- Pins are written as **Part PIN**: `ESP32 GPIO21`, `PCA9685 V+`, `OLED SCK`. Use the label printed on the board.
- Chip/part numbers in their correct case: ESP32, PCA9685, SSD1306, MG90S, I2C, USB-C, GPIO.
- New part? Add it to this table and give it a page in `components/`.

## 3. Project structure

Every project lives in `projects/<short-kebab-name>/`:

```
projects/<name>/
├── README.md       follows templates/project-readme.md, in that order
├── diagrams/       wiring diagram(s), one .html each
├── firmware/       code (PlatformIO: platformio.ini + src/)
└── media/          photos/clips, small, location metadata stripped
```

The project README sections, **always in this order** (see `templates/project-readme.md`):

1. **What it does**: one or two sentences, and a photo if there is one.
2. **Components needed**: table: component, qty, notes. Then "Tools needed".
3. **Power**: where power comes from, each rail's voltage and worst-case current, and why it's safe.
4. **Wiring diagram**: link to `diagrams/…html` + a wire table (From → To) matching the diagram.
5. **Build steps**: numbered. Each step = what to do, then a ✓ check (what to measure, between which two points, expected reading, what to do if wrong).
6. **Code**: how to flash (one command) and what the code does.
7. **Test result**: what was measured and the outcome.
8. **What went wrong**: real mistakes, symptoms, how they were found and fixed.
9. **Learnings**: links to `notes/…` pages this project produced.
10. **Next steps**.

After a project, also:
- Add it to the table in the root `README.md`.
- Move reusable learnings into `notes/<topic>/` (one file per idea) and link them.
- Add any new part to `components/` and `resources/inventory.md`.

## 4. Wiring diagram style

One self-contained `.html` file per diagram: inline SVG, no external images or scripts.
Example to copy: `projects/esp32-servo-robot/diagrams/esp32_2servo_wiring.html`.

**Page**
- About 1600 px wide (`viewBox="0 0 1600 …"`), light/white background, readable printed on A4 landscape
  (`@page { size: A4 landscape; margin: 6mm; }`).
- Title at the top: parts + one-line summary of the power design.
- Left column: **numbered wire list** in the order to connect them, then a **"Leave unconnected"** list.
- Show **every** component, pin, wire and rail. Pin labels match the board silkscreen.
- Include a **legend**, a **⚠ "Can damage parts"** box and a **✓ Checks** box.

**Wire colours** (use the same meaning in every diagram)

| Meaning | Colour | Width |
|---|---|---|
| 5 V motor/servo power | red `#c92a2a` | thick 5 |
| 5 V logic power | red `#c92a2a` | thin 2.8 |
| Battery / raw input (VBAT) | orange `#e67700` | thick 5 |
| Ground, motor path | black `#1b1b1b` | thick 5 |
| Ground, logic | black `#1b1b1b` | thin 2.6 |
| 3V3 logic | green `#2f9e44` | 2.6 |
| I2C SDA | purple `#7048e8` | 2.6 |
| I2C SCL | blue `#1971c2` | 2.6 |
| Other signals | grey `#495057` | 1.6 |

**Boxes**: module body fill `#eef2f6`, outline `#343a40`; warning box fill `#fff5f5`, outline `#c92a2a`;
checks box fill `#ebfbee`, outline `#2f9e44`.

**Rules**
- **Joined wires get a dot. Crossing wires get a hop (small arc) and no dot.** Never draw a plain "+" crossing.
- Thick lines only where high current flows, so the current path is visible at a glance.
- Label removable jumpers (e.g. `J1`) and say when to pull them.
- Mark pins that must stay unconnected with ✕.

## 5. Safety (always include)

- State the worst-case current for each power rail and show it's under the supply's rating.
- Keep motor current out of the microcontroller board.
- Say which mistakes can destroy parts: wrong voltage on VCC, reversed electrolytic capacitors,
  two supplies connected together, hot-plugging servos.

## 6. Public repo hygiene

- No secrets: WiFi passwords and keys go in `secrets.h` (git-ignored); commit `secrets.example.h`.
- Link datasheets, courses and other people's diagrams in `resources/links.md`; don't copy them in.
- Strip location metadata from photos.
- Commits: one per step or learning, clear message (`notes: why GPIO12 breaks boot`),
  no AI co-author lines.

## 7. Licence

Code: MIT. Notes, diagrams, images: CC BY 4.0. See `LICENSE`.
