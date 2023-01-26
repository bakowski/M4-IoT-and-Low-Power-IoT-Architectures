#include <Wire.h>

//  may be used with Wire, Wire1, ..

uint16_t readLightLevel(void) 
  {
  uint16_t level;
  Wire.beginTransmission(0x23);
  Wire.write(0x01);  // power on
  Wire.endTransmission();delay(50);
  Wire.beginTransmission(0x23);
  Wire.write(0x13);  // high resolution - 120 ms, 0x13 - low resolution - 20 ms
  Wire.endTransmission();
  delay(50);   // high resolution - delay(200);  
  Wire.beginTransmission(0x23);
  Wire.requestFrom( 0x23, 2);
  level = Wire.read();
  level <<= 8;
  level |= Wire.read();
  Wire.endTransmission();delay(40);
  level = level/1.2; // convert to lux
  return level;
}

void setup()
{
    Serial.begin(9600);
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);delay(50);
}

void loop()
{
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);  delay(50);
    Wire.begin();
    Serial.print("Luminosity(lux): ");
    Serial.println(readLightLevel());
    Wire.end();
    digitalWrite(Vext, HIGH);
    delay(1000);
}
