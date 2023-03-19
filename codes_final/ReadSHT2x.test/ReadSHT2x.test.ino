/****************************************************************
 * ReadSHT2x
 *  An example sketch that reads the sensor and prints the
 *  relative humidity to the PC's serial port
 ***************************************************************/

#include <Wire.h>
#include <SHT2x.h>

SHT2x sht;
void setup()
{
    Serial.begin(9600);
}

void loop()
{
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
    delay(50);

    Wire.begin();
    Serial.print("Humidity(%RH): ");
    Serial.print(sht.getHumidity());
    Serial.print("     Temperature(C): ");
    Serial.println(sht.getTemperature());
    Wire.end();

    digitalWrite(Vext, HIGH);

    delay(3000);
}
