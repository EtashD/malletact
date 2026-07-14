// Day 3-4 — PWM LED Fade + KY-037 Mic Analog Read
//
// LED:  GPIO2 -> resistor -> LED -> GND
// Mic:  KY-037 G -> GND | + -> 3V3 | AO -> GPIO34

#define LED_PIN 2
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8
#define MIC_PIN 34

// PWM fade state
int brightness = 0;
int stepSize = 5;
unsigned long lastFadeStep = 0;
const unsigned long fadeInterval = 15;

// Mic state
int smoothed = 0;
unsigned long lastMicRead = 0;
const unsigned long micInterval = 20;

void setup() {
  Serial.begin(115200);
  delay(500);
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RESOLUTION);
  Serial.println("Day 3-4: PWM fade + KY-037 mic test starting...");
}

void loop() {
  unsigned long now = millis();

  // Fade LED
  if (now - lastFadeStep >= fadeInterval) {
    lastFadeStep = now;
    ledcWrite(LED_PIN, brightness);

    brightness += stepSize;
    if (brightness <= 0 || brightness >= 255) {
      stepSize = -stepSize;
    }
  }

  // Read + smooth mic
  if (now - lastMicRead >= micInterval) {
    lastMicRead = now;
    int raw = analogRead(MIC_PIN);
    smoothed = (smoothed * 0.8) + (raw * 0.2);
    Serial.println(smoothed);
  }
}
