#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <SHT21.h>  // include SHT21 library
SHT21 sht;
#include <BH1750.h>
BH1750 bh;


#define timetillsleep 1200
#define timetillwakeup 5000
static TimerEvent_t sleep;
static TimerEvent_t wakeUp;
uint8_t lowpower=1,highpower=0;

void onSleep()
{
  Serial.printf("\nGoing into lowpower mode, %d ms later wake up.\r\n",timetillwakeup);
  lowpower=1;highpower=0;
  //timetillwakeup ms later wake up;
  TimerSetValue( &wakeUp, timetillwakeup );
  TimerStart( &wakeUp );
}
void onWakeUp()
{
  Serial.printf("\nWoke up, %d ms later into lowpower mode.\r\n",timetillsleep);
  lowpower=0;highpower=1;
  //timetillsleep ms later into lowpower mode;
  TimerSetValue( &sleep, timetillsleep );
  TimerStart( &sleep );
}

typedef enum
{
  RS, TX, RX   // 3 states : Read Sensor, Transmit, Receive
} states_t;

states_t state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Radio.Sleep( ); state=RS;
  TimerInit( &sleep, onSleep );
  TimerInit( &wakeUp, onWakeUp );
  onSleep();
}


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

union pack
{
 uint8_t frame[16]; // trames avec octets
  float  data[4];   // 4 valeurs en virgule flottante
} sdp ;  // paquet d’émission

void loop() {
  if(lowpower){
    lowPowerHandler();
  }
  if(highpower)
    {
    switch(state)
      {
        case RS: Serial.println("state: RS VT:");
                 sdp.data[0]=(float)read_Bat();Serial.println(sdp.data[0]);
                 sdp.data[1]=(float)read_Temp();Serial.println(sdp.data[1]);
                 sdp.data[2]=(float)read_Humi();Serial.println(sdp.data[2]);
                 sdp.data[3]=(float)read_Lumi();Serial.println(sdp.data[3]);
                 delay(100); state=TX; break;
        case TX: Serial.println("state: TX");delay(400); state=RX; break;
        case RX: Serial.println("state: RX");delay(400); state=RS; break;
      }
    }
  // put your main code here, to run repeatedly:
}
