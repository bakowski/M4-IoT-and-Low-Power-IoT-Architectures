
#include "Arduino.h"
#include <SHT21.h>  // include SHT21 library
SHT21 sht;
static uint16_t counter=0;

//Some utilities for going into low power mode
TimerEvent_t sleepTimer;
//Records whether our sleep/low power timer expired
bool sleepTimerExpired;

static void wakeUp()
{
  sleepTimerExpired=true;
}

static void lowPowerSleep(uint32_t sleeptime)
{
  sleepTimerExpired=false;
  TimerInit( &sleepTimer, &wakeUp );
  TimerSetValue( &sleepTimer, sleeptime );
  TimerStart( &sleepTimer );
  //Low power handler also gets interrupted by other timers
  //So wait until our timer had expired
  while (!sleepTimerExpired) lowPowerHandler();
  TimerStop( &sleepTimer );
}

///////////////////////////////////////////////////

union pack
{
 uint8_t frame[16]; // trames avec octets
  float  data[4];   // 4 valeurs en virgule flottante
} sdp ;  // paquet d’émission

uint16_t read_Bat()
{
  uint16_t v;
  pinMode(VBAT_ADC_CTL,OUTPUT);
  digitalWrite(VBAT_ADC_CTL,LOW);
  v=analogRead(ADC)+550; //*2;
  pinMode(VBAT_ADC_CTL, INPUT);
  return v;
}

float read_Temp()
{
  float t;
  pinMode(Vext, OUTPUT); 
  digitalWrite(Vext, LOW);  
  Wire.begin();delay(100);
  t = sht.getTemperature();  // get temp from SHT
  return t;
}
float read_Lumi()
{
  float l;
  bh.begin();
  delay(100);
  l = bh.readLightLevel();
  return l;
}

float read_Humi()
{
  float h;
  delay(100);
  h = sht.getHumidity();  // get temp from SHT
  return h;
}

void setup() 
{
	Serial.begin(9600);

}

int ttsleep=10000;

void loop()
{
  counter++; 
  Serial.printf("\nGoing to sleep with time_to_sleep=%d\n", ttsleep);delay(100);
  lowPowerSleep(ttsleep);  
  Serial.printf("\nBack from sleep %d, counter=%d\n", millis(),counter);
  sdp.data[0]=(float)read_Bat();Serial.println(sdp.data[0]);
  sdp.data[1]=(float)read_Temp();Serial.println(sdp.data[1]);
  sdp.data[2]=(float)read_Humi();Serial.println(sdp.data[2]);
  sdp.data[3]=(float)read_Lumi();Serial.println(sdp.data[3]);
}
