//==============================================================================
//    E - R A D I O N I C A . C O M,  H.Kolomana 6/A, Djakovo 31400, Croatia
//==============================================================================
// Project   :  SHT21 Arduino Library
// File      :  SHT21 Demo Arduino Example
// Author    :  e-radionica.com 2017
// Licence   :  Open-source ! 
//==============================================================================
//==============================================================================
// Use with any SHT21 breakout. Check ours: 
// https://e-radionica.com/en/sht21-humidity-and-temperature-sensor.html
// Shows temperature and humidity from SHT21 sensor in Serial monitor.
// If any questions, 
// just contact techsupport@e-radionica.com
//==============================================================================

#include <SHT21.h>  // include SHT21 library
#include "HT_SSD1306Wire.h"

SHT21 sht; 

float temp;   // variable to store temperature
float humi; // variable to store hemidity

SSD1306Wire  display(0x3c, 100000, SDA, SCL, GEOMETRY_128_64, -1);
                    
void displayOLED(char *line1, char *line2, char *line3)
{ 
    Serial.println("in oled");
    display.init();
    display.flipScreenVertically();display.clear();
    display.drawString(20, 50, "SmartComputerLab" );
    display.drawString(0, 0,  line1 );
    display.drawString(0, 15, line2);
    display.drawString(0, 30, line3);
    display.display();
    delay(1000);
}


void setup() {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW); delay(100); 
  Wire.begin();   // begin Wire(I2C)
  Serial.begin(9600); // begin Serial
}

void loop() {
char buff[32],bufft[32],buffh[32];
  temp = sht.getTemperature();  // get temp from SHT 
  humi = sht.getHumidity(); // get temp from SHT

  Serial.print("Temp: ");     // print readings
  Serial.print(temp);
  Serial.print("\t Humidity: ");
  Serial.println(humi);
  sprintf(bufft,"T:%d",(int)temp);sprintf(buffh,"H:%d",(int)humi);
  displayOLED(bufft,buffh," ");
  delay(85);  // min delay for 14bit temp reading is 85ms
}
