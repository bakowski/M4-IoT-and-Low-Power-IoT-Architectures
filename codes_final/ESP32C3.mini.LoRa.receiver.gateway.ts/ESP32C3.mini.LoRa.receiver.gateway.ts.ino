//Attention low upload speed- 460800

#include <LoRa.h>
#include <Wire.h>               

#include <WiFiManager.h> 
#include "ThingSpeak.h"

unsigned long myChannelNumber =1626377;   
const char *myWriteAPIKey="3IN09682SQX3PT4Z" ;
const char *myReadAPIKey="9JVTP8ZHVTB9G4TT" ;

WiFiClient  client;

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
#define BAND    8685E5
int sf=9;
long sbw=125E3;
typedef union 
{
 uint8_t frame[16]; // frames with bytes
  float  sensor[4];   // 4 floating point values
} pack_t ;  // packet type

QueueHandle_t dqueue;  // queues for data packets

void onReceive(int packetSize) 
{ 
pack_t rdp,sdp;  // receive buffer with pack_t format
int i=0;   
  if (packetSize == 0) return;   // if there's no packet, return 
  i=0; 
  if (packetSize==16)   // 
    { 
    while (LoRa.available()) {  rdp.frame[i]=LoRa.read();i++;  } 
      xQueueReset(dqueue); // to keep only the last element 
      xQueueSend(dqueue, &rdp,portMAX_DELAY ); //portMAX_DELAY
    } 
} 

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
  Serial.begin(9600);
  WiFi.mode(WIFI_STA); 
  WiFiManager wm;
  // wm.resetSettings();
  bool res;
  res = wm.autoConnect("ESP32AP",NULL); // no password  
  if(!res) Serial.println("Failed to connect");// ESP.restart(); 
  else  Serial.println("connected...yeey :)");
      
  ThingSpeak.begin(client); // connexion (TCP) du client au serveur
  delay(1000);
  Serial.println("ThingSpeak begin"); 

  SPI.begin(SCK, MISO, MOSI, SS);  // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);

  delay(1000);
  Serial.println();Serial.println();
  Serial.println("Starting LoRa Receiver");
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
   dqueue = xQueueCreate(4,16); // queue for 4 data packets
   LoRa.onReceive(onReceive);  // pour indiquer la fonction ISR
   LoRa.receive();             // pour activer l'interruption (une fois)
   }
}

uint32_t mindel=15000;  // 15 seconds
long lastsent=0;

void loop() 
{
pack_t rp,sdp;    // packet elements to send
  xQueueReceive(dqueue,rp.frame,portMAX_DELAY); // 6s,default:portMAX_DELAY 
  delay(1000);
  LoRa.beginPacket();  // sending ACK packet
  sdp.sensor[0]=(float)random(60,80);sdp.sensor[1]=(float)random(10,80);
  LoRa.write(sdp.frame,16);
  LoRa.endPacket();
  LoRa.receive();
  if(millis()> lastsent+15000)
    {
    ThingSpeak.setField(1,rp.sensor[0]); 
    ThingSpeak.setField(2,rp.sensor[1]); 
    ThingSpeak.setField(3,rp.sensor[2]); 
    ThingSpeak.setField(4,rp.sensor[3]); 
    Serial.printf("d1=%2.2f,d2=%2.2f,d3=%2.2f,d4=%2.2f\n",rp.sensor[0],rp.sensor[1], rp.sensor[2],rp.sensor[3]);
    while (WiFi.status() != WL_CONNECTED) { delay(500);  } 
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200){Serial.println("Channel update successful.");}
    else { Serial.println("Problem updating channel. HTTP error code " + String(x));} 
    lastsent=millis();
    }  
}
