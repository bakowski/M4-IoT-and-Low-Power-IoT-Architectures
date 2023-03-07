#include "Arduino.h"
#include "LoRa_APP.h"
#include <Wire.h>
#include <SHT21.h>
#define timetillsleep 5000
#define timetillwakeup 10000
static TimerEvent_t sleep;
static TimerEvent_t wakeUp;
uint8_t lowpower=1, highpower=0;
SHT21 sht; 

float temp;   // variable to store temperature
float humi; // variable to store hemidity

void getSHT21()
{
    pinMode(Vext, OUTPUT); digitalWrite(Vext, LOW);delay(50);
    Wire.begin(); delay(40);
    temp = sht.getTemperature();  // get temp from SHT 
    humi = sht.getHumidity(); // get temp from SHT
    Serial.print("Humidity(%RH): ");Serial.print(humi);
    Serial.print("     Temperature(C): ");Serial.println(temp);
    Wire.end();delay(40);
    digitalWrite(Vext, HIGH);delay(40);
}


void onSleep()
{
  Serial.printf("Going into lowpower mode, %d ms later wake up.\r\n",timetillwakeup);
  lowpower=1;highpower=0;
  TimerSetValue( &wakeUp, timetillwakeup ); //timetillwakeup ms later wake up;
  TimerStart( &wakeUp );
}

void onWakeUp()
{
  Serial.printf("Woke up, %d ms later into lowpower mode.\r\n",timetillsleep);
  lowpower=0;highpower=1;
  //timetillsleep ms later into lowpower mode;
  TimerSetValue( &sleep, timetillsleep );
  TimerStart( &sleep );
}

void setup() {
  Serial.begin(9600);
  Radio.Sleep( );
  TimerInit( &sleep, onSleep ); TimerInit( &wakeUp, onWakeUp );
  onSleep();
}

void loop() {
  if(lowpower){
    lowPowerHandler();
  }
  if(highpower)
     { getSHT21();highpower=0; }
}
