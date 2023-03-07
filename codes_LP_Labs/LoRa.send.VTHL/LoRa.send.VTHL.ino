#include "LoRaWan_APP.h"
#include "Arduino.h"

#include <BH1750.h>
BH1750 lightMeter;

#include <SHT21.h>  // include SHT21 library
SHT21 sht; 

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

typedef enum
{
    LOWPOWER,  ReadVTHL,  TX   // 3 states (1,2,3)
} States_t;

States_t state;
bool sleepMode = false;
int16_t rssi,rxSize;
uint16_t voltage;

float temperature, humidity, luminosity;

union pack
{
 uint8_t frame[16]; // trames avec octets
  float  data[4];   // 4 valeurs en virgule flottante
} sdp ;  // paquet d’émission

void setup() 
{
    Serial.begin(9600); delay(200);
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW); delay(100); 
    Wire.begin();   
    voltage = 0;
    rssi=0;
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
    state=ReadVTHL;   // read voltage , temperature and humidity
}


void loop()
{
  switch(state)
  {
    case TX:
    {
      sdp.data[0] = (float)voltage;
      sdp.data[1] = temperature;
      sdp.data[2] = humidity;
      sdp.data[3] = luminosity;
      if(voltage<(uint16_t)3680)turnOnRGB(COLOR_SEND,0);
      else turnOnRGB(COLOR_RECEIVED,200);
      Serial.printf("\r\nsending packet- mV:%d, T:%d, H:%d, L:%d\n",voltage,(int)temperature,(int)humidity,(int)luminosity);
      Radio.Send(sdp.frame,16);
      Serial.println(strlen(txPacket));delay(100);
      state=LOWPOWER;
      break;
    }
    case LOWPOWER:
    {
      lowPowerHandler();delay(100);
      turnOffRGB();
      delay(2000);  //LowPower time
      state = ReadVTHL; 
      break;
    }
    case ReadVTHL:
    {
      pinMode(VBAT_ADC_CTL,OUTPUT);
      digitalWrite(VBAT_ADC_CTL,LOW);
      voltage=analogRead(ADC)+550; //*2;
      pinMode(VBAT_ADC_CTL, INPUT);
      pinMode(Vext, OUTPUT);delay(40);
      digitalWrite(Vext, LOW); delay(40);
      Wire.begin();delay(40);
      temperature = sht.getTemperature();  // get temp from SHT 
      humidity = sht.getHumidity(); // get temp from SHT
      Serial.print("Temp: ");     // print readings
      Serial.print(temperature);
      Serial.print("\t Humidity: ");
      Serial.println(humidity);delay(40);
      digitalWrite(Vext,LOW); // start power before activating Wire
      Wire.begin();delay(100);   
      lightMeter.begin(); delay(200);   // 200
      luminosity = lightMeter.readLightLevel();
      Serial.print("Light: ");
      Serial.print(luminosity);
      Serial.println(" lux");
      delay(40);
      Wire.end();delay(40);
      digitalWrite(Vext, HIGH); delay(40);
      state = TX;
      break;
    }
     default:
          break;
  }
  Radio.IrqProcess();
}

void OnTxDone( void )
{
  Serial.print("TX done!");
  turnOnRGB(0,0);
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    state=ReadVTHL;
    Serial.print(state);
}
