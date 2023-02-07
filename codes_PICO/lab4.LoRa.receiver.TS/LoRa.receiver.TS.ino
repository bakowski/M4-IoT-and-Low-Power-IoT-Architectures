//Attention low upload speed- 460800

#include <LoRa.h>
#include <WiFiManager.h> 
#include "ThingSpeak.h"
unsigned long myChannelNumber = 1697981;   
const char *myWriteAPIKey="A1G48I8FNSKGLRUB" ;  //SmartIoTBox-3
WiFiClient  client;
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 8, 10);   // ADDRESS, SDA, SCL

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

void disp(char *t1, char *t2, char *t3)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, t1);
  display.drawString(0, 16, t2);
  display.drawString(0, 32, t3);
  display.display();
}

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  Serial.println();Serial.println();
  WiFi.mode(WIFI_STA); 
  WiFiManager wm;
    // wm.resetSettings();
    disp("Starting","WEB portal at","192.168.4.1");
    bool res;
    res = wm.autoConnect("ESP32AP",NULL); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }   
  ThingSpeak.begin(client); // connexion (TCP) du client au serveur
  delay(1000);
  Serial.println("ThingSpeak begin");
  
  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);

  Serial.println("Starting LoRa Receiver");
  // Initialising the UI will init the display too.

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    disp("Connect","LoRa modem","failed");
    while (1);
  }
  else 
   {
   Serial.println("Starting LoRa ok!");

   disp("Connect","LoRa modem","OK");
   LoRa.setSpreadingFactor(sf);
   LoRa.setSignalBandwidth(sbw);
   }
}

union 
{
  uint8_t frame[64];
  char mess[64];
  float sensor[8];
} rdp;



float temperature=0.0,humidity=0.0;

void loop() {
char buff[32],brssi[32];
int i=0,rssi=0;
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      rdp.frame[i] = LoRa.read();i++;
    }
  if(packetSize==64)
      {
      sprintf(buff,"T:%2.2f,H:%2.2f",rdp.sensor[0],rdp.sensor[1]);
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
      rssi = LoRa.packetRssi();
      sprintf(brssi,"RSSI: %d",rssi);
      disp("Received packet:",buff,brssi);
      Serial.println("Fields update");
      ThingSpeak.setField(1, rdp.sensor[0]);
      ThingSpeak.setField(2, rdp.sensor[1]);
      int x=ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200) Serial.println("Channel update successful.");
      else Serial.println("Problem updating channel. HTTP error code " + String(x));
      delay(16000);
      }
  }
}
