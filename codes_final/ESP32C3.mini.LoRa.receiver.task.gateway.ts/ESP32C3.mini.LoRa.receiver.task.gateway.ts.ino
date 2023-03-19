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

QueueHandle_t ts_queue;  // queues for data packets
int rssi=0;

void ParseTask( void * pvParameters )
{
  pack_t rdp,sdp;
  while(true)
  {
    int packetLen; char b1[32];
    packetLen=LoRa.parsePacket();
    if(packetLen==16)
    {
      int i=0;
      while (LoRa.available()) {
      rdp.frame[i]=LoRa.read();i++;
    } 
    rssi=LoRa.packetRssi(); 
    delay(1000);
    LoRa.beginPacket();  // sending ACK packet
    sdp.sensor[0]=(float)random(60,80);sdp.sensor[1]=(float)random(10,80);
    LoRa.write(sdp.frame,16);
    LoRa.endPacket();
  
    xQueueReset(ts_queue); // to keep only the last element 
    xQueueSend(ts_queue, &rdp, portMAX_DELAY); 
    }
  }
}

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
   ts_queue = xQueueCreate(4,16); // queue for 4 data packets
   xTaskCreate(
                    ParseTask,   /* Function to implement the task */
                    "ParseTask", /* Name of the task */
                    10000,      /* Stack size in words */
                    NULL,       /* Task input parameter */
                    0,          /* Priority of the task */
                    NULL);       /* Task handle. */
  Serial.println("Task Parse created...");
   }
}

uint32_t mindel=15000;  // 15 seconds
long lastsent=0;

void loop() 
{
pack_t rp;     
char d1[32],d2[32],d3[32],d4[32], d5[32];

  xQueueReceive(ts_queue,rp.frame,portMAX_DELAY); // 6s,default:portMAX_DELAY 

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
    sprintf(d1,"Battery (mV): %2.2f",rp.sensor[0]);sprintf(d2,"Temperature : %2.2f",rp.sensor[1]);
    sprintf(d3,"Humidity : %2.2f",rp.sensor[2]);sprintf(d4,"Luminosity : %2.2f",rp.sensor[3]);
    sprintf(d5,"RSSI: %d",rssi);
    disp(d1,d2,d3,d4,d5);
    lastsent=millis();
    }  
}
