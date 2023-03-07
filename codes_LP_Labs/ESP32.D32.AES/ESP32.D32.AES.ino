#include "mbedtls/aes.h"

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

void setup() {

  mbedtls_aes_context aes;
  Serial.begin(9600);delay(400);
  Serial.println(); Serial.println();
  
  //char key[16]= { 0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03,0x00, 0x01, 0x02, 0x03};
  char *key = "abcdefghijklmnop";
  unsigned char input[16] = { 0xAB, 0xCD, 0xAC, 0xB8,0x8A, 0x77, 0xA6, 0xA6,0x8B, 0xC1, 0xD2, 0xF3,0xBB, 0xF1, 0xF2, 0xF3};
  //unsigned char *input = (unsigned char *)"SmartComputerLabSmartComputerLab";
  unsigned char crypte[16], decrypte[16];
  
  Serial.println();  Serial.println();delay(400);
  for(int i=0; i<16;i++) Serial.print(input[i],HEX);
  Serial.println();
  encrypt(input, key, crypte, 1);
  for(int i=0; i<16;i++) Serial.print(crypte[i],HEX);
  Serial.println();delay(400);
  Serial.println();  
  decrypt(crypte, key, decrypte,1); 
  for(int i=0; i<16;i++) Serial.print(decrypte[i],HEX);
  Serial.println();delay(400);
}

void loop() {
  // put your main code here, to run repeatedly:
}
