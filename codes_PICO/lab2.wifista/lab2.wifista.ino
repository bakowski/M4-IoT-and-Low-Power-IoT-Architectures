#include "WiFi.h"
#include <Wire.h>               
#include "SSD1306Wire.h"         
SSD1306Wire display(0x3c, 8, 10);  

void display_SSD1306(char *text) 
{
  char buff[256];
  strcpy(buff,text);
  display.init();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "ETN - WiFi SCAN");
  display.drawString(0, 14, buff);
  display.display();
}

void setup()
{
  char buf[16];
  Serial.begin(9600);
  Wire.begin(8,10);
  Serial.println();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(2000);
  Serial.println("Setup done");
}
