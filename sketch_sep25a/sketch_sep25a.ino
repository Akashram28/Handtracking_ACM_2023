#include <Adafruit_NeoPixel.h>

#define LED_PIN 13 // Pin connected to the Data Input of the LED strip
#define LED_COUNT 97 // Number of LEDs in your strip

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int counter = 0;
const int baudRate = 9600;

void setup() {
  strip.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(baudRate);
  for (int i = 1; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
  }
}

void loop() {
  if (Serial.available()) {
    int numBlinks = Serial.parseInt();
    if (numBlinks >= 1){
      strip.clear();
    }
    for (int i = 9; i > 0; i--) {
      strip.setPixelColor(numBlinks*9-i, strip.Color(0, 255, 0));
      strip.show();
    }
    numBlinks = 0;
  }
  delay(100);
}
