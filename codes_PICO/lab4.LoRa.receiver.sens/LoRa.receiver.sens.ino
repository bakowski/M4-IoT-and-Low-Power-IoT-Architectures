//Attention low upload speed- 460800

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
   }
}

void loop() {
int i=0, rssi;
char bt1[32],bt2[32],bt3[32];
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize==64) {
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) {
      rdp.frame[i] = LoRa.read();i++;
    }
    rssi=LoRa.packetRssi();
    Serial.println(rdp.sensor[0]);
    Serial.println(rdp.sensor[1]);
    Serial.println(rdp.sensor[2]);
    Serial.println(rdp.sensor[3]);
    sprintf(bt1,"S1:%2.2f, S2:%2.2f",rdp.sensor[0],rdp.sensor[1]);
    sprintf(bt2,"S3:%2.2f, S4:%2.2f",rdp.sensor[2],rdp.sensor[3]);
    sprintf(bt3,"RSSI:%d",rssi);
    disp("LoRa packet",bt1,bt2,bt3);
    Serial.print("' with RSSI ");
    Serial.println(rssi);
  }

}
