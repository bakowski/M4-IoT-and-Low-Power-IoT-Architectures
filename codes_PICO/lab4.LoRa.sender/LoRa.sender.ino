//Attention low upload speed- 460800

#include <LoRa.h>
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

union 
{
  uint8_t frame[64];
  char mess[64];
  float sensor[8];
} sdp;

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

void setup() {
  Serial.begin(9600);
  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);

  Serial.begin(9600);
  delay(1000);
  Serial.println();Serial.println();
  Serial.println("Starting LoRa Sender");
  // Initialising the UI will init the display too.
  display.init();
  display.flipScreenVertically();
  if (!LoRa.begin(BAND)) {
    disp("start","LoRa failed","end");
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else 
   {
   Serial.println("Starting LoRa ok!");
   disp("start","LoRa OK","end");
   LoRa.setSpreadingFactor(sf);
   LoRa.setSignalBandwidth(sbw);
   }
   sdp.sensor[0]=0.0;
   sdp.sensor[1]=0.0;
   sdp.sensor[2]=0.0;
   sdp.sensor[3]=0.0;
}



int counter=0;

void loop() {
char buff[32];
  Serial.print("Sending packet: ");
  Serial.println(counter);
  sprintf(buff,"Count:  %d",counter);
  disp("Sending packet",buff,"   ");
  // send packet
  LoRa.beginPacket();
  LoRa.write(sdp.frame,64);
//  LoRa.print("hello ");
//  LoRa.print(counter); 
  LoRa.endPacket();
  sdp.sensor[0]+=0.1;
  sdp.sensor[1]+=0.2;
  sdp.sensor[2]+=0.3;
  sdp.sensor[3]+=0.4;
  counter++;
  delay(5000);

}
