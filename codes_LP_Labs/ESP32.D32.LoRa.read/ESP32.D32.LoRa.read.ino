#include <SPI.h>               
#include <LoRa.h>
#include <WiFi.h>
#include <Wire.h>                
#include "SSD1306Wire.h"         
SSD1306Wire display(0x3c, 12, 14); 
#include "ThingSpeak.h"
char* ssid     = "PhoneAP";
char* pass  = "smartcomputerlab";
WiFiClient client;

unsigned long myChannelNumber = 114;   // Thinspeak channel
const char *myWriteAPIKey="E8KYBCRD2Z59LVWJ" ;  // write code

#define SCK     18   // GPIO18 -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    23   // GPIO23 -- SX127x's MOSI
#define SS       5   // GPIO05 -- SX127x's CS
#define RST     15   // GPIO15 -- SX127x's RESET
#define DI0     26   // GPIO26 -- SX127x's IRQ(Interrupt Request)
#define freq    8685E5   
#define sf      9
#define sb      125E3  

union 
{
  uint8_t frame[64]; // trames avec octets
  char   mess[64];   // text message
  float  sensor[8];    // 4 valeurs en virgule flottante
} rdp;

void disp(char *d1,char *d2,char *d3) 
{
  display.init();
  //display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_16);  // ArialMT_Plain_10
  display.drawString(0, 0, d1);
  display.drawString(0, 18, d2);
  display.drawString(0, 36, d3);
  display.setFont(ArialMT_Plain_10);
  display.drawString(20, 52, "SmartComputerLab");
  display.display();
}

void setup() {
  Serial.begin(9600);                  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  Serial.println();delay(100);Serial.println();
  if (!LoRa.begin(freq)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
Serial.println("Starting LoRa OK!");
LoRa.setSpreadingFactor(sf);
LoRa.setSignalBandwidth(sb);
}


void loop()   
{
int packetLen, rssi;
packetLen=LoRa.parsePacket();
if(packetLen)
  {
  int i=0; char tbuff[32];
  while (LoRa.available()){rdp.frame[i]=LoRa.read();i++;}
  rssi=LoRa.packetRssi();  // force du signal en réception en dB 
  Serial.printf("Received packet:%s\n",rdp.mess+12);
  Serial.printf("RSSI=%d\n",rssi); sprintf(tbuff,"RSSI= %d",rssi);
  disp(rdp.mess+12,tbuff,"  ");delay(1000);
  }
}
