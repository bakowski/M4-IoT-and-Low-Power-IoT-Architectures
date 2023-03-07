
#include <SHT21.h>  // include SHT21 library

SHT21 sht; 

float temperature;   // variable to store temperature
float humidity; // variable to store hemidity

void setup() {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); delay(100); 
  Wire.begin();   // begin Wire(I2C)
  Serial.begin(9600); // begin Serial
}

void loop() {
  pinMode(Vext, OUTPUT);delay(100);
  digitalWrite(Vext, LOW); delay(100);
  Wire.begin();delay(50);
  humidity = sht.getHumidity(); // get temp from SHT
  temperature = sht.getTemperature();  // get temp from SHT 
  humidity = sht.getHumidity(); // get temp from SHT
  delay(100);
  Wire.end();delay(50);
  digitalWrite(Vext, HIGH); delay(100);
  
  Serial.print("Temp: ");     // print readings
  Serial.print(temperature);
  Serial.print("\t Humidity: ");
  Serial.println(humidity);
  delay(85);  // min delay for 14bit temp reading is 85ms
}
