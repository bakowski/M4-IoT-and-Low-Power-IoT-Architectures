#include <LoRa.h>
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

// Initialize the OLED display using Arduino Wire:
SSD1306Wire display(0x3c, 8, 10);   // ADDRESS, SDA, SCL

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
union 
{
  uint8_t frame[64];
  char mess[64];
  float sensor[8];
} sdp,rdp;

void disp(char *t1, char *t2, char *t3, char *t4)
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, t1);
  display.drawString(0, 16, t2);
  display.drawString(0, 32, t3);
  display.drawString(0, 48, t4);
  display.display();
}

void setup() {
  Serial.begin(115200);
  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);
  Serial.begin(9600);
  delay(1000);
  Serial.println();Serial.println();
  Serial.println("Starting LoRa Sender");
  display.init();
  display.flipScreenVertically();
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else 
   {
   Serial.println("Starting LoRa ok!");
   LoRa.setSpreadingFactor(sf);
   LoRa.setSignalBandwidth(sbw);
   LoRa.onReceive(onReceive);  // pour indiquer la fonction ISR
   LoRa.receive();             // pour activer l'interruption (une fois)
   // puis après chaque émission
   }
}

void onReceive(int packetSize) 
{ 
int rssi=0; 
union 
{
  uint8_t frame[64];
  char mess[64];
  float sensor[8];
} sdp,rdp;

  if (packetSize == 0) return;   // if there's no packet, return 
  int i=0; 
  if (packetSize==64) 
    { 
    while (LoRa.available()) 
      { 
      rdp.frame[i]=LoRa.read();i++; 
      } 
      rssi=LoRa.packetRssi(); 
      Serial.printf("Received packet:%2.2f,%2.2f\n",rdp.sensor[0],rdp.sensor[1]);
      Serial.printf("RSSI=%d\n",rssi);
    } 
}

void loop()
{
Serial.println("in the loop") ;
delay(5000) ;
}
