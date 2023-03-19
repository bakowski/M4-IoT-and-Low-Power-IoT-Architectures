#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "aes.h"
aes_context ctx[2];
uint8_t in[16]={ 0xAB, 0xCD, 0xAC, 0xB8,0x8A, 0x77, 0xA6, 0xA6,0x8B, 0xC1, 0xD2, 0xF3,0xBB, 0xF1, 0xF2, 0xF3};
uint8_t out[16], out1[16];
const uint8_t key[16]={ 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,0x69, 0x6A, 0x6B, 0x6C,0x6D, 0x6E, 0x6F, 0x70};

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
#define BUFFER_SIZE                                 30 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

float txNumber;
bool lora_idle=true;


typedef union 
{
 uint8_t frame[16]; // trames avec octets
  float  data[4];   // 4 valeurs en virgule flottante
  char   mess[16];
} pack_t;

pack_t sdp ;  // paquet d’émission




void setup(){
  Serial.begin(9600);delay(300);
    txNumber=0;

    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 

      aes_set_key(key,
                 16,         // len in bytes - length_type
                ctx );
                  
}

void loop()
{ 
  char *mess="smartcomputerlab";
  if(lora_idle == true)
  {
    memcpy(in,(uint8_t *)mess,16); 
    Serial.println("Clear packet");delay(300);
    Serial.println(mess);
    for(int i=0; i<16;i++) Serial.print(in[i],HEX);
    Serial.println();delay(300);               
    aes_encrypt(in,
                out1,
                ctx );            
    Serial.println("Sent crypted packet");
    for(int i=0; i<16;i++) Serial.print(out1[i],HEX);
    Serial.println();
    memcpy(sdp.frame,out1,16);
    Radio.Send( (uint8_t *)sdp.frame, 16 ); //send the package out 
    lora_idle = false;
    Serial.println();delay(300);
    Serial.println();
    delay(3500);
  }
}

void OnTxDone( void )
{
  turnOffRGB();
  Serial.println("TX done......");
  lora_idle = true;
}

void OnTxTimeout( void )
{
  turnOffRGB();
  Radio.Sleep( );
  Serial.println("TX Timeout......");
  lora_idle = true;
}
