#include <SoftwareSerial.h> 
SoftwareSerial ss(7, 6); 
    
void loop()
{
  while (ss.available() > 0)
  Serial.write(ss.read());
}
    
void setup()
{ 
  Serial.begin(115200);
  ss.begin(9600);//start softserial for GPS at defined baud rate
}
