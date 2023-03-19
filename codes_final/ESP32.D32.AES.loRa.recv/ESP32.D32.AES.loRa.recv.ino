#include <SPI.h>               
#include <LoRa.h>
#include "mbedtls/aes.h"

#define SCK     18   // GPIO18 -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    23   // GPIO23 -- SX127x's MOSI
#define SS       5   // GPIO05 -- SX127x's CS
#define RST     15   // GPIO15 -- SX127x's RESET
#define DI0     25   // GPIO25 (integrated modem)  -- SX127x's IRQ(Interrupt Request)
#define freq    8685E5   
#define sf 9
#define sb 125E3  


typedef union 
{
 uint8_t frame[16]; 
 float  data[4];
 char     mess[16];   
} pack_t;

pack_t rdp;  

void encrypt(unsigned char *plainText,char *key,unsigned char *outputBuffer, int nblocks)
{
  mbedtls_aes_context aes;
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_enc(&aes,(const unsigned char*)key,strlen(key)*8);
  for(int i=0;i<nblocks;i++)
    {
    mbedtls_aes_crypt_ecb(&aes,MBEDTLS_AES_ENCRYPT,
                        (const unsigned char*)(plainText+i*16), outputBuffer+i*16);
    }                 
  mbedtls_aes_free(&aes);
}

void decrypt(unsigned char *chipherText,char *key,unsigned char *outputBuffer, int nblocks)
{
  mbedtls_aes_context aes;
  mbedtls_aes_init( &aes );
  mbedtls_aes_setkey_dec( &aes, (const unsigned char*) key, strlen(key) * 8 );
    for(int i=0;i<nblocks;i++)
    {
    mbedtls_aes_crypt_ecb(&aes,MBEDTLS_AES_DECRYPT,
                       (const unsigned char*)(chipherText+i*16), outputBuffer+i*16);
    }                   
  mbedtls_aes_free(&aes );
}

void setup() 
{
  mbedtls_aes_context aes;
  Serial.begin(9600);  
  delay(1000);                  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  Serial.println();delay(100);Serial.println();
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
Serial.println("Starting LoRa OK!");
delay(1000);
LoRa.setSpreadingFactor(sf);
LoRa.setSignalBandwidth(sb);
LoRa.setCodingRate4(5);
}

void loop() {
  //char key[16]= { 0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03};
  char *key = "abcdefghijklmnop";
  unsigned char input[16] = { 0xAB, 0xCD, 0xAC, 0xB8,0x8A, 0x77, 0xA6, 0xA6,0x8B, 0xC1, 0xD2, 0xF3,0xBB, 0xF1, 0xF2, 0xF3};
  //unsigned char *input = (unsigned char *)"SmartComputerLabSmartComputerLab";
  unsigned char crypte[16], decrypte[16];
  int packetLen; char b1[32];
  packetLen=LoRa.parsePacket();
  if(packetLen==16)
  {
  int i=0;
  while (LoRa.available()) {
    rdp.frame[i]=LoRa.read();i++;
    } 
  memcpy(crypte,rdp.frame,16);
  
//  Serial.println();  Serial.println();delay(400);
//  for(int i=0; i<16;i++) Serial.print(input[i],HEX);
//  Serial.println();
//  encrypt(input, key, crypte, 1);
  Serial.println("\nReceived crypted packet");
  for(int i=0; i<16;i++) Serial.print(crypte[i],HEX);
  Serial.println();delay(400);  
  decrypt(crypte, key, decrypte,1); 
  Serial.println("Decrypted packet");
  for(int i=0; i<16;i++) Serial.print(decrypte[i],HEX);
  Serial.println();delay(400);
  Serial.printf("%16.16s\n",decrypte);
  }
}
