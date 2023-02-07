#include <WiFiManager.h>
#include "time.h"
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 12 
#define PIN 6   // build-in LED - 7, RGB LED ring - 6
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

void setup(){
    WiFi.mode(WIFI_STA); 
    Serial.begin(115200); 
    pixels.begin();
    pixels.setPixelColor(0, pixels.Color(0, 150, 0));
    pixels.show();
    WiFiManager wm;
    // wm.resetSettings();
    bool res;
    res = wm.autoConnect("ESP32C3AP",NULL); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  for(int i=0;i<12;i++)pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  pixels.show();
}

void loop(){
  delay(1000);
  printLocalTime();
}

/*
 * The tm structure contains a calendar date and time broken down into its components:

    tm_sec: seconds after the minute;
    tm_min: minutes after the hour;
    tm_hour: hours since midnight;
    tm_mday: day of the month;
    tm_year: years since 1900;
    tm_wday: days since Sunday;
    tm_yday: days since January 1;
    tm_isdst: Daylight Saving Time flag;
 */

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  Serial.print("Day of week: ");
  Serial.println(&timeinfo, "%A");
  Serial.print("Month: ");
  Serial.println(&timeinfo, "%B");
  Serial.print("Day of Month: ");
  Serial.println(&timeinfo, "%d");
  Serial.print("Year: ");
  Serial.println(&timeinfo, "%Y");
  Serial.print("Hour: ");
  Serial.println(&timeinfo, "%H");
  Serial.print("Hour (12 hour format): ");
  Serial.println(&timeinfo, "%I");
  pixels.setPixelColor(timeinfo.tm_hour, pixels.Color(150, 0, 0));
  pixels.show();
  Serial.print("Minute: ");
  Serial.println(&timeinfo, "%M");
  Serial.print("Second: ");
  Serial.println(&timeinfo, "%S");
  Serial.println();
}
