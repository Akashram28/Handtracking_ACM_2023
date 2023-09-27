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
    uint32_t color = rgb(i/(double)LED_COUNT);
    strip.setPixelColor(i, color);
    strip.show();
  }
}

void loop() {
  if (Serial.available()) {
    int numBlinks = Serial.parseInt();
    
    if (numBlinks >= 1){
      Serial.println(numBlinks);
      strip.clear();
    }
    for (int i = 9; i > 0; i--) {
      uint32_t color = rgb((numBlinks*9-i)/(double)LED_COUNT);

      Serial.println(color);
      strip.setPixelColor(numBlinks*9-i, color);
      
    }
    // for(int i=0;i<numBlinks;i++){
    //   digitalWrite(LED_BUILTIN, HIGH);
    //   delay(500);
    //   digitalWrite(LED_BUILTIN, LOW);
    //   delay(500);
    // }
    // strip.show();
    numBlinks = 0;
  }
  delay(100);
}



uint32_t rgb(double ratio) {
  // Normalize the ratio so that it fits into 9 regions, each 256 units long
  int normalized = int(ratio * 256 * 9);

  // Find the region for this position
  int region = normalized / 256;

  // Find the distance to the start of the closest region
  int x = normalized % 256;

  uint8_t r = 0, g = 0, b = 0;
  switch (region) {
    case 0: r = 255; g = 0;   b = 0;   g += x; break;
    case 1: r = 255; g = 85;  b = 0;   r -= x; break;
    case 2: r = 255; g = 170; b = 0;   g += x; break;
    case 3: r = 170; g = 255; b = 0;   r += x; break;
    case 4: r = 85;  g = 255; b = 0;   g -= x; break;
    case 5: r = 0;   g = 255; b = 0;   b += x; break;
    case 6: r = 0;   g = 255; b = 85;  r -= x; break;
    case 7: r = 0;   g = 255; b = 170; g += x; break;
    case 8: r = 0;   g = 170; b = 255; r += x; break;
  }
  return r + (g << 8) + (b << 16);
}


// uint32_t rgb(double ratio)
// {
//     //we want to normalize ratio so that it fits in to 6 regions
//     //where each region is 256 units long
//     int normalized = int(ratio * 256 * 6);

//     //find the region for this position
//     int region = normalized / 256;

//     //find the distance to the start of the closest region
//     int x = normalized % 256;

//     uint8_t r = 0, g = 0, b = 0;
//     switch (region)
//     {
//     case 0: r = 255; g = 0;   b = 0;   g += x; break;
//     case 1: r = 255; g = 255; b = 0;   r -= x; break;
//     case 2: r = 0;   g = 255; b = 0;   b += x; break;
//     case 3: r = 0;   g = 255; b = 255; g -= x; break;
//     case 4: r = 0;   g = 0;   b = 255; r += x; break;
//     case 5: r = 255; g = 0;   b = 255; b -= x; break;
//     }
//     return r + (g << 8) + (b << 16);
// }

