# NN · Project name

<!-- Copy this file to projects/<name>/README.md. Keep the sections in this order.
     Style rules: see AGENTS.md in the repo root. -->

## What it does

One or two sentences. Add a photo from `media/` if you have one.

## Components needed

| Component | Qty | Notes |
|---|---|---|
| ESP32-WROOM-32 DevKit, 38-pin | 1 | |
| | | |

**Tools needed:** multimeter, …

## Power

- **Supply:** what powers it, and its rating (e.g. 5 V 2 A power bank = 10 W).
- **Rails:**

| Rail | Voltage | Feeds | Worst-case current |
|---|---|---|---|
| | | | |

- **Why it's safe:** worst case total vs. supply rating, in one line.

## Wiring diagram

Open [diagrams/NAME.html](diagrams/NAME.html) (prints on A4 landscape).

| # | From | To | Notes |
|---|---|---|---|
| 1 | | | |

**Leave unconnected:** …

⚠ **Can damage parts:** …

## Build steps

1. **Do:** …
   ✓ **Check:** meter on DC volts, X to Y: expect … **If wrong:** …
2. …

## Code

```bash
cd firmware && ~/.platformio/penv/bin/pio run -t upload -t monitor
```

What the code does, in 3–5 bullets.

## Test result

| Check | Expected | Measured |
|---|---|---|
| | | |

## What went wrong

- **Symptom:** … **Cause:** … **How I found it:** … **Fix:** …

## Learnings

- [Note title](../../notes/topic/note.md)

## Next steps

- …
