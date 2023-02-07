#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp;
  
void setup() {
  Serial.begin(9600);
  Wire.begin(8,10);
  bmp.begin();  
}
  
void loop() {
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature()); Serial.println(" *C");   
    Serial.print("Pressure = "); Serial.print(bmp.readPressure());
    Serial.println(" Pa");    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");
    // you can get a more precise measurement of altitude
    // if you know the current sea level pressure which will
    // vary with weather and such. If it is 1015 millibars
    // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(104300));  Serial.println(" meters");    
    Serial.println();
    delay(5000);
}
