#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;

void setup()
{
  Serial.begin(9600);
  pinMode(Vext,OUTPUT);delay(100);
  Wire.begin();
}

float lux;
void loop() 
{
  digitalWrite(Vext,LOW); // start power before activating Wire
  delay(200);
  lightMeter.begin();
  delay(200);
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  Wire.end();  // end Wire before disconnecting power
  digitalWrite(Vext,HIGH);
  delay(3000);
}
