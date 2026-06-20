# Day 1 — Blink (June 19, 2026)
Parts arrived today

## What I built
A basic LED blink circuit controlled by the ESP32. The LED turns on for 1 second, off for 1 second, repeatedly.

## Wiring
- GPIO2 (D2) → resistor → LED long leg
- LED short leg → GND
- Resistor limits current to protect the LED

## Circuit logic
D2 outputs 3.3V when HIGH and 0V when LOW. Current flows out of D2, through the resistor, through the LED, and returns to GND — completing the circuit. Without the GND return wire the circuit is open and the LED won't light.

## Issues hit
- Port was grayed out in Arduino IDE — fixed by installing CP210x Universal Windows Driver
- Blink sketch used LED_BUILTIN which isn't defined for ESP32 — replaced with pin number 2
- LED wasn't lighting up after upload — missing GND return wire

## Code
```cpp
void setup() {
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}
```
## Photos & Videos
https://drive.google.com/drive/folders/1pbBmBVNa123nracsDoDV7-ifSFqyFI62?usp=sharing

## Next
Button input + PWM dimming
