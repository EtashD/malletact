# Day 5 — Sound-Reactive ERM Motor (November 2026)

## What I built

The first version of the signal path the whole project runs on: KY-037 ambient mic → ESP32 ADC → envelope detection → PWM → ERM coin motor. Sound in, vibration out. Everything before this was the pieces practiced separately. This is the first time they run as one circuit.

Picked this back up after a pause over the fall for college applications.

## Wiring

**KY-037 mic (unchanged from Day 4):**

- KY-037 `G` → ESP32 GND
- KY-037 `+` → ESP32 3V3
- KY-037 `AO` → ESP32 GPIO34 (ADC1_CH6)

**Motor:** driven from GPIO27 under PWM.

All female-to-female jumpers gripping component legs and pin headers directly, no breadboard.

## Circuit logic

**The motor can't run off the pin directly.** An ESP32 GPIO is rated around 12mA sustained; an ERM coin motor pulls 60–100mA in motion and more at stall. It needs a 2N2222 as a low-side switch, so the GPIO only supplies base current through a 1kΩ resistor while the motor's actual current comes off the 3V3 rail through the collector. The 3V3 rail and not VIN, because the motor is 3V nominal and 3.7V absolute max.

**It also needs a flyback diode.** When the transistor switches off, the motor's coil field collapses and kicks a reverse voltage spike back down the line. A 1N4001 across the motor terminals, striped end to motor +, gives that spike a loop to burn off in instead of the transistor or the ESP32.

**PWM.** `ledcAttach(MOTOR_PIN, 5000, 8)` — 5kHz carrier, 8-bit resolution, 0–255 duty. Switching that fast means the motor sees an average voltage rather than discrete on/off pulses, which turns duty into an intensity control.

**Envelope detection.** `analogRead(34)` returns raw amplitude samples and they jitter hard. A 4-sample rolling average smooths that first. On top of it, a self-calibrating envelope tracks a running min and max of the smoothed signal and relaxes both toward each other by `decayRate 0.999` every pass, so the loud/quiet reference keeps adapting to the room instead of sitting on a fixed threshold that only works in one space. The current sample gets scaled inside that window and mapped to 0–255 duty.

Serial output is a single `duty` value per line.

## Issues hit

**Killed a mic module.** Wired the header wrong, and bent one of the pins getting it back off. Straightening it didn't save it. Swapped in a spare KY-037 from the 4-pack. These modules are cheap enough that this isn't a real loss, but it costs a session, and the fix is just reading the pin labels against the board before anything gets seated.

**The ERM motors need soldering.** The coin motors from the original order ship with bare pads, no leads attached. Soldering isn't a skill I want on the critical path this early in the build, so I ordered replacements that come with leads already on. The motor side stays as-is until those arrive.

**Duty numbers still aren't right.** The self-calibrating envelope keeps the motor moving, but the mapping doesn't correspond to how loud the room actually is. Two things to measure rather than guess at: the duty value where this specific motor breaks static friction and starts spinning (that becomes the output floor, everything below it is dead range), and whether `decayRate 0.999` relaxes too fast for music with quiet passages.

**No frequency separation yet.** Everything above is broadband amplitude, one motor reacting to total loudness. Four motors with four different jobs is the whole design, and none of that exists in the firmware right now. See Next.

## Photos and Videos

https://drive.google.com/drive/folders/1pbBmBVNa123nracsDoDV7-ifSFqyFI62?usp=drive_link

## Next

Confirm the driver stage is fully built out — 2N2222 and 1N4001 both in the line — and measure the duty value where the motor starts spinning, so the output floor is set from a real number instead of a guess.

Then Day 6, frequency separation. Right now the mic signal is collapsed into one amplitude number, and splitting it into bands is what turns this from a buzzer into the actual instrument. Plan is an FFT on raw ADC samples, binned into three bands for the vest motors, with the wrist motor running onset detection instead of a band.

Tag `v0.1` once a single channel is calibrated and holds up through a wear test.
