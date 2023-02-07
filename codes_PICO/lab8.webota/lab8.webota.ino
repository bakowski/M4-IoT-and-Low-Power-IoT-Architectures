#include <WebOTA.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 1 
#define PIN 7   // build-in LED - 7 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const char* host     = "ESP-OTA"; // Used for MDNS resolution
const char* ssid = "Livebox-08B0";
const char* password = "G79ji6dtEptVTPWmZP";

void setup() {
  Serial.begin(115200);
  pixels.begin();
  for(int i=0;i<12;i++)pixels.setPixelColor(i, pixels.Color(0, 150, 0));
  pixels.show();
  init_wifi(ssid, password, host);
  // Defaults to 8080 and "/webota" , webota.init(80, "/update");
}

void loop() {
  int md = 5000;
  pixels.setPixelColor(0, pixels.Color(0, 150, 0)); pixels.show();
  webota.delay(md);
  pixels.setPixelColor(0, pixels.Color(0, 0, 150)); pixels.show();
  webota.delay(md);
  webota.handle();
}
