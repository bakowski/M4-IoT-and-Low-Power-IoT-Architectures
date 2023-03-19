#include <LoRa.h>
#include <Wire.h>               

#include <WiFiManager.h> 
#include "ThingSpeak.h"

//unsigned long myChannelNumber =1697980;   
//const char *myWriteAPIKey="4K897XNNHTW7I4NO" ;
//const char *myReadAPIKey="V9GT1RN2FIP0YRAY" ;

WiFiClient  client;

#include <U8x8lib.h>
U8X8_SSD1306_128X64_NONAME_SW_I2C display(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// with LoRa modem RFM95 and green RFM board - int and RST to solder

#define SS      18 // 26     // D0 - to NSS
#define RST     4  //16     // D4  - RST
#define DI0     26      // D8 - INTR

#define SCK     18      // D5 - CLK
#define MISO    19     // D6 - MISO
#define MOSI    23      // D7 - MOSI
#define BAND    8685E5
int sf=9;
long sbw=125E3;
typedef union 
{
 uint8_t frame[36]; // frames with bytes
 struct 
 {
 unsigned int channel; // 4 bytes - device and channel identifier
 char   wkey[16];      // 16 bytes
 float  sensor[4];     // 16 bytes
 } pay;                // payload
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
    if(packetLen==36)
    {
      int i=0;
      while (LoRa.available()) {
      rdp.frame[i]=LoRa.read();i++;
    } 
    rssi=LoRa.packetRssi(); 
    delay(1000);
    LoRa.beginPacket();  // sending ACK packet
    sdp.pay.sensor[0]=(float)random(60,80);sdp.pay.sensor[1]=(float)random(10,80);
    LoRa.write(sdp.frame,36);
    LoRa.endPacket();
  
    xQueueReset(ts_queue); // to keep only the last element 
    xQueueSend(ts_queue, &rdp, portMAX_DELAY); 
    }
  }
  }

void disp(char *d1,char *d2,char *d3,char *d4,char *d5) 
{
  display.begin();display.clear();
  display.setFont(u8x8_font_chroma48medium8_r);
  display.drawString(0, 0, d1);
  display.drawString(0, 1, d2);
  display.drawString(0, 2, d3);
  display.drawString(0, 3, d4);
  display.drawString(0, 4, d5);
  display.drawString(0, 7, "SmartComputerLab");
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

  SPI.begin(5, 19, 27, 18); // SCK, MISO, MOSI, SS
  LoRa.setPins(SS, RST, DI0);

  delay(1000);
  Serial.println();Serial.println();
  Serial.println("Starting LoRa Receiver");
  display.begin();
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  else 
   {
   Serial.println("Starting LoRa ok!");
   LoRa.setSpreadingFactor(sf);
   LoRa.setSignalBandwidth(sbw);
   ts_queue = xQueueCreate(4,36); // queue for 4 data packets
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
    ThingSpeak.setField(1,rp.pay.sensor[0]); 
    ThingSpeak.setField(2,rp.pay.sensor[1]); 
    ThingSpeak.setField(3,rp.pay.sensor[2]); 
    ThingSpeak.setField(4,rp.pay.sensor[3]); 
    ThingSpeak.setField(5,rssi); 
    Serial.printf("d1=%2.2f,d2=%2.2f,d3=%2.2f,d4=%2.2f\n",rp.pay.sensor[0],rp.pay.sensor[1], rp.pay.sensor[2],rp.pay.sensor[3]);
    while (WiFi.status() != WL_CONNECTED) { delay(500);  } 
    int x = ThingSpeak.writeFields(rp.pay.channel, rp.pay.wkey);
    if(x == 200){Serial.println("Channel update successful.");}
    else { Serial.println("Problem updating channel. HTTP error code " + String(x));} 
    sprintf(d1,"B(mV): %2.2f",rp.pay.sensor[0]);sprintf(d2,"Temp: %2.2f",rp.pay.sensor[1]);
    sprintf(d3,"Humi: %2.2f",rp.pay.sensor[2]);sprintf(d4,"Lumi: %2.2f",rp.pay.sensor[3]);
    sprintf(d5,"RSSI: %d",rssi);
    disp(d1,d2,d3,d4,d5);
    lastsent=millis();
    }  
}
