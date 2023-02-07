#include "WiFi.h"
void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println();
  Serial.print("My MAC address is: ");
  Serial.println(WiFi.macAddress());
}
 
void loop()
{
}
