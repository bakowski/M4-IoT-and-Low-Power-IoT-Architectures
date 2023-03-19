
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <SHT21.h>  // include SHT21 library
SHT21 sht;
#include <BH1750.h>
BH1750 bh;
static uint16_t counter=0;

#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif
#define RF_FREQUENCY                                868500000 // Hz
#define TX_OUTPUT_POWER                             14        // dBm
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       9         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false
#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 128 // Define the payload size here

char txPacket[BUFFER_SIZE];
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );

int16_t rssi;


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
  rssi=0;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  Radio.Init( &RadioEvents );
//  Radio.SetChannel( RF_FREQUENCY );
//  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
//                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
//                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
//                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
  

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
