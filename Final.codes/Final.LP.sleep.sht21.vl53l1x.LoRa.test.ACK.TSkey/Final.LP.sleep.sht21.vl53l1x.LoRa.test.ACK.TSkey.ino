
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <SHT21.h>  
SHT21 sht;
#include "SparkFun_VL53L1X.h" 
//Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X
SFEVL53L1X distanceSensor;

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
#define RX_TIMEOUT_VALUE                            2000
//#define BUFFER_SIZE                                 128 // Define the payload size here

static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnRxDone( void );
void OnTxTimeout( void );
void OnRxTimeout( void );

int16_t rssi,rxSize;
uint8_t sent=0;


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

typedef union 
{
 uint8_t frame[36]; // frames with bytes
 struct 
 {
 unsigned int channel; // 4 bytes - device and channel identifier
 char   wkey[16];      // 16 bytes
 float  sensor[4];     // 16 bytes
 } pay;                // payload
} pack_t ;  // packet type

pack_t sdp;  // paquet d’émission

uint16_t read_Bat()
{
  uint16_t v;
  delay(40);
  pinMode(VBAT_ADC_CTL,OUTPUT);
  digitalWrite(VBAT_ADC_CTL,LOW);
  v=analogRead(ADC)+550; //*2;
  pinMode(VBAT_ADC_CTL, INPUT);
  return v;
}

float read_Temp()
{
  float t;
  t = sht.getTemperature();  // get temp from SHT
  return t;
}

int distance=0;

void read_Dist()
{
  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1) ;
  }
  Serial.println("Sensor VL53L1X online!");
  //distanceSensor.setDistanceModeShort();
  distanceSensor.setDistanceModeLong();
  distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
  distanceSensor.clearInterrupt();
  distanceSensor.stopRanging();
}

float read_Humi()
{
  float h;

  h = sht.getHumidity();  // get temp from SHT
  return h;
}

unsigned long myChannelNumber =1538804;   
const char *myWriteAPIKey="YOX31M0EDKO0JATK" ;
const char *myReadAPIKey="20E9AQVFW7Z6XXOM" ;

void setup() 
{
	Serial.begin(9600);
  rssi=0;
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.RxTimeout = OnRxTimeout;
  
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                  true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
                                  
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

}


int ttsleep=20000;
bool lora_idle = true;

void loop()
{
  counter++; 
  Serial.printf("\ntime_to_sleep=%d ms\n", ttsleep);delay(100);
  lowPowerSleep(ttsleep);  
  Serial.printf("\nBack from sleep %d, counter=%d\n", millis(),counter);
  sdp.pay.sensor[0]=(float)read_Bat();Serial.println(sdp.pay.sensor[0]);
  pinMode(Vext, OUTPUT); delay(100);
  digitalWrite(Vext, LOW);  delay(100);
  Wire.begin();delay(100);
  sdp.pay.channel=myChannelNumber;
  memcpy(sdp.pay.wkey,myWriteAPIKey,16);
  sdp.pay.sensor[1]=(float)read_Temp();Serial.println(sdp.pay.sensor[1]);delay(100);
  sdp.pay.sensor[2]=(float)read_Humi();Serial.println(sdp.pay.sensor[2]);delay(100);
  read_Dist();
  sdp.pay.sensor[3]=(float)distance;Serial.println(sdp.pay.sensor[3]);delay(100);
  
  Wire.end();  delay(100);
  digitalWrite(Vext, HIGH); delay(100); 
  turnOnRGB(COLOR_SEND,0);
  Radio.Send((uint8_t *)sdp.frame,36); 
  delay(400);  // min sending time before the reception
  lora_idle = true;
  while(lora_idle)  // waiting for ACK packet or timeout
    {
    turnOffRGB();
    Serial.println("ACK wait");
    lora_idle = false;
    Radio.Rx(0);
    }

}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  pack_t rp;
    memcpy(rp.frame, payload, size );
    turnOnRGB(COLOR_RECEIVED,0);
    Radio.Sleep( );
    Serial.printf("\nrssi=%d,length=%d,id=%u,timeout=%d\n",rssi,size,rp.pay.channel,(int)rp.pay.sensor[0]);
    ttsleep = (int)rp.pay.sensor[0]*200;delay(100);
    lora_idle = false; //true;
    turnOffRGB();
}

void OnTxDone( void )
{
  Serial.print("TX done!\n");
  turnOnRGB(0,0);
  lora_idle = true;
}

void OnTxTimeout( void )
{
    turnOffRGB();
    Radio.Sleep( );
    Serial.print("TX Timeout......");
}

void OnRxTimeout( void )
{
    turnOnRGB(COLOR_RECEIVED,0);
    Radio.Sleep( );
    Serial.print("RX Timeout......");delay(100);
    lora_idle = false; //true;
    turnOffRGB();
}
