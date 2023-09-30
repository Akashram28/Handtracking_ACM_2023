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
  strip.setBrightness(50);
  for (int i = 1; i < LED_COUNT; i++) {
    uint32_t color = rgb(i);
    strip.setPixelColor(i, color);
    strip.show();
  }
}

void loop() {
  if (Serial.available()) {
    int numBlinks = Serial.parseInt();
    strip.show();
    if (numBlinks >= 1){
      Serial.println(numBlinks);
      strip.clear();
      // strip.show();
    }
    
    for (int i = 9; i > 0; i--) {
      uint32_t color = rgb(numBlinks*9-i);

      // Serial.println(color);
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



uint32_t rgb(int num) {
  if (num<=30){
    double ratio = (double)255/30;
    return strip.Color(255,(int) num*ratio,0);
  }
  if (num>30 && num<=40){
    return strip.Color(255-(num-30)*24,255,0);
  }
  if (num>40 && num<=60){
    return strip.Color(0,255-12*(num-40),12*num);
  }
  if (num>60 && num<=75){
    return strip.Color(8*(num-60),0,255);
  }
  else{
    return strip.Color(128+8*(num-75),0,255);
  }

}



