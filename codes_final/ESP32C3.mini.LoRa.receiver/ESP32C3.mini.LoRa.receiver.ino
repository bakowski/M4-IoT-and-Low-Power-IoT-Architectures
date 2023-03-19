//Attention low upload speed- 460800

#include <LoRa.h>
#include <Wire.h>   
#include <WiFi.h>             
#include "ThingSpeak.h"
#include "SSD1306Wire.h"         

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 8, 10);   // ADDRESS, SDA, SCL


const char* ssid     = "Livebox-08B0";
const char* password = "G79ji6dtEptVTPWmZP";

unsigned long myChannelNumber = 1697980;
const char * myWriteAPIKey = "4K897XNNHTW7I4NO";

String receivedText;
String receivedRssi;
// with LoRa modem RFM95 and green RFM board - int and RST to solder

#define SS      5 // 26     // D0 - to NSS
#define RST     3  //16     // D4  - RST
#define DI0     2      // D8 - INTR

#define SCK     1      // D5 - CLK
#define MISO    0      // D6 - MISO
#define MOSI    4      // D7 - MOSI
#define BAND    868E6
int sf=7;
long sbw=125E3;

char dbuff[24];

WiFiClient  client;

void setup() {
  Serial.begin(9600);

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
//  delay(500);
//  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  delay(1000);   
  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);
  
  Serial.println();Serial.println();
  Serial.println("Starting LoRa Receiver");


  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "Starting LoRa failed");
    display.display();
    while (1);
  }
  else 
   {
   Serial.println("Starting LoRa ok!");
   display.clear();
   display.setFont(ArialMT_Plain_10);
   display.drawString(0, 0, "Starting LoRa OK");
   display.display();
   LoRa.setSpreadingFactor(sf);
   LoRa.setSignalBandwidth(sbw);
   }
}

void loop() {
char buff[32];
char currentid[64];
char receivedText[64]; int i=0;
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    display.clear(); memset(receivedText,0x00,64);
    Serial.print("Received packet '");
    display.drawString(0, 2, "Received packet");
    sprintf(buff,"Packet size:%d",packetSize);
    display.drawString(0, 16, buff);
    display.display();
    // read packet
    while (LoRa.available()) {
      receivedText[i] = (char)LoRa.read();i++;
      //receivedText.toCharArray(currentid, 64);
    }
    Serial.print(receivedText);
    if(packetSize<24)
      {
      display.drawString(0, 32, receivedText);
      display.display();
      }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    receivedRssi = LoRa.packetRssi();
    char currentrs[64];
    receivedRssi.toCharArray(currentrs, 64);
    sprintf(buff,"RSSI: %s",currentrs);
    display.drawString(0, 48, buff);
    display.display();
  }

}
