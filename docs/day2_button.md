# Day 2 — Button Input (June 25, 2026)

## What I built
A push button circuit that turns the LED on when pressed and off when released.

## Wiring
- D4 → button leg (one side)
- Button other leg → GND via negative rail
- D2 → LED long leg → resistor → GND via negative rail
- Negative rail connected to GND pin on ESP32
- Resistor and LED wiring were adjusted to allow for the activation of the negative rail

## Circuit logic
INPUT_PULLUP ties D4 internally to 3.3V, so it reads HIGH when unpressed 
and LOW when pressed (button connects it to GND). The sketch turns the LED 
on when it reads LOW and off when it reads HIGH.

## Issues hit
- Button was reading LOW constantly — both wires were on the same side of 
  the button (row 60). Fixed by moving GND wire to the opposite side (row 62).

## Code
See day2_button.ino in firmware/ramp_up/

# Photos and Videos
https://drive.google.com/drive/folders/1pxdheA2vZyIc2eCXpLjCQnB3s_ak71AY?usp=sharing

## Next
PWM dimming — fade LED brightness up and down using ledcWrite(), 
same technique used later for haptic motor intensity control
