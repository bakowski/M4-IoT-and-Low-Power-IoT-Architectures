#include <WiFi.h>
#include "ThingSpeak.h"
#include <SPI.h>               
#include <LoRa.h>
#include <Wire.h>  
#include "SSD1306Wire.h"         
SSD1306Wire display(0x3c, 12, 14); 

const char* ssid     = "Livebox-08B0";
const char* password = "G79ji6dtEptVTPWmZP";

WiFiClient  client;
unsigned long myChannelNumber = 1697980;
const char * myWriteAPIKey = "4K897XNNHTW7I4NO";

#define SCK     18   // GPIO18 -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    23   // GPIO23 -- SX127x's MOSI
#define SS       5   // GPIO05 -- SX127x's CS
#define RST     15   // GPIO15 -- SX127x's RESET
#define DI0     25   // GPIO25 (integrated modem)  -- SX127x's IRQ(Interrupt Request)
#define freq    8685E5   
#define sf 9
#define sb 125E3  

union pack
{
 uint8_t frame[16]; // trames avec octets
  float  data[4];   // 4 valeurs en virgule flottante
} rdp,sdp ;  // paquet d’émission

void disp(char *d1,char *d2,char *d3,char *d4,char *d5) 
{
  display.init();
  //display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);  // ArialMT_Plain_10
  display.drawString(0, 0, d1);
  display.drawString(0, 9, d2);
  display.drawString(0, 18, d3);
  display.drawString(0, 27, d4);
  display.drawString(0, 36, d5);
  display.setFont(ArialMT_Plain_10);
  display.drawString(20, 52, "SmartComputerLab");
  display.display();
}

void setup() {
  Serial.begin(9600);  
  delay(1000); 
  Serial.print("[WiFi] Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) 
    {
      Serial.print(".");
      delay(500);
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  delay(1000);
  Wire.begin(12,14);                 
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

int rssi;

void loop()   
{
int packetLen; char b1[32];
char d1[32],d2[32],d3[32],d4[32], d5[32];
packetLen=LoRa.parsePacket();
if(packetLen==16)
  {
  int i=0;
  while (LoRa.available()) {
    rdp.frame[i]=LoRa.read();i++;
    }
  rssi=LoRa.packetRssi();  // force du signal en réception en dB 
  Serial.printf("V:%2.2f,T:%2.2f,H:%2.2f,L:%2.2f\n",rdp.data[0],rdp.data[1],rdp.data[2],rdp.data[3]);
  Serial.printf("RSSI=%d\n",rssi);sprintf(b1,"V:=%4.2f",rdp.data[0]);
  disp("received","packet",b1," "," ");
  delay(1000);
  LoRa.beginPacket();
  sdp.data[0]=(float)random(40,80);sdp.data[1]=(float)random(10,80);
  LoRa.write(sdp.frame,16);
  LoRa.endPacket();
  disp("sent","packet","ACK","for ","data  ");
  ThingSpeak.setField(1, rdp.data[0]);
  ThingSpeak.setField(2, rdp.data[1]);
  ThingSpeak.setField(3, rdp.data[2]);
  ThingSpeak.setField(4, rdp.data[3]);
  ThingSpeak.setField(5, rssi);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000);
  sprintf(d1,"Battery (mV): %2.2f",rdp.data[0]);sprintf(d2,"Temperature : %2.2f",rdp.data[1]);
  sprintf(d3,"Humidity : %2.2f",rdp.data[2]);sprintf(d4,"Luminosity : %2.2f",rdp.data[3]);
  sprintf(d5,"RSSI: %d",rssi);
  disp(d1,d2,d3,d4,d5);
  delay(10000);
  }
}
