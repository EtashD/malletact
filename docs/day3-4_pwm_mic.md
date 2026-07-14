# Day 3–4 — PWM Dimming + KY-037 Microphone Input (July 2026)

## What I built
Day 3: A PWM fade sketch that smoothly brightens and dims the Day 1 LED using the ESP32's LEDC peripheral.
Day 4: A sketch that reads the KY-037 ambient mic's analog output and prints live values to Serial Monitor, confirmed responding to claps, taps, and music.

## Wiring

**Day 3 (LED, unchanged from Day 1):**
- GPIO2 → resistor → LED → GND

**Day 4 (KY-037 mic):**
- KY-037 `G` → ESP32 GND
- KY-037 `+` → ESP32 3V3
- KY-037 `AO` → ESP32 GPIO34 (ADC1_CH6)
- Direct jumper wires, off-breadboard (see issues below)

## Circuit logic

**Day 3:** `ledcAttach(pin, freq, resolution)` sets up PWM on GPIO2 at 5kHz, 8-bit resolution (0–255 duty range). The loop increments `brightness` by `stepSize` each pass, reversing direction at 0 and 255, so the LED fades up and down continuously. `stepDelay` controls how fast the fade moves.

**Day 4:** `analogRead(34)` reads the mic's analog voltage output. A weighted running average (`smoothed = smoothed*0.8 + raw*0.2`) is layered on top to reduce raw ADC jitter into something that tracks volume more like a wave than a jagged spike pattern.

## Issues hit

**Day 3:** None — confirmed ESP32 Arduino core 3.3.10 uses the merged `ledcAttach()` call instead of the older `ledcSetup()` + `ledcAttachPin()` pair from core 2.x.

**Day 4:**
- GPIO34 read flat 0 on the breadboard, even after fixing an early miswiring (mic `G` was on the wrong/positive rail).
- Root cause: the ESP32 DOIT board's plastic housing overhangs the breadboard, blocking columns b–e under the chip — only column a is reachable, and the pins of the ESP32 itself already take it.
- Also found the breadboard's column strips split at a horizontal gap around row 30 — a wire below the gap isn't the same node as a pin above it.
- A direct 3.3V test confirmed GPIO34's row had no live connection, while GPIO32 read fine — looked like a dead pin, but wasn't.
- Fix: freed up a second ESP32 and wired the KY-037 directly to its pin headers with female-to-female jumpers, bypassing the breadboard entirely. GPIO34 worked immediately once off-board — confirms it was a breadboard geometry issue, not a bad pin.
- Sensitivity: mic picks up claps, taps, and music well, but barely registers quiet continuous tones like humming — expected for this module's small capsule/preamp. Sometimes there isn't consistency with many claps/taps in a row.

## Code
See `day3-4_pwm.ino` in `firmware/ramp_up/`

## Photos and Videos
https://drive.google.com/drive/folders/1pxdheA2vZyIc2eCXpLjCQnB3s_ak71AY?usp=sharing

## Next
Day 5 — swap the LED for an ERM motor (2N2222 + 1N4001 flyback) and build out amplitude envelope detection so the motor pulses with the beat.
