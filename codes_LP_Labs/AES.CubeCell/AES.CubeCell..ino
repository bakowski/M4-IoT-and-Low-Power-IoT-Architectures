#include "aes.h"
aes_context ctx[2];
const uint8_t in[16]={ 0xAB, 0xCD, 0xAC, 0xB8,0x8A, 0x77, 0xA6, 0xA6,0x8B, 0xC1, 0xD2, 0xF3,0xBB, 0xF1, 0xF2, 0xF3};
uint8_t out[16], out1[16];
const uint8_t key[16]={ 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,0x69, 0x6A, 0x6B, 0x6C,0x6D, 0x6E, 0x6F, 0x70};

void setup(){
  Serial.begin(9600);delay(300);
  Serial.println("startcrypt");delay(300);
  aes_set_key(key,
                 16,         // len in bytes - length_type
                ctx );
  for(int i=0; i<16;i++) Serial.print(in[i],HEX);
  Serial.println();delay(300);               
  aes_encrypt(in,
              out1,
              ctx );            
  for(int i=0; i<16;i++) Serial.print(out1[i],HEX);
  Serial.println();delay(300);
  aes_decrypt(out1,
              out,
              ctx );
for(int i=0; i<16;i++) Serial.print(out[i],HEX);
Serial.println();                  
}

void loop(){ delay(15000);}
