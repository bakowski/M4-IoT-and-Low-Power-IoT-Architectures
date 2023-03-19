#include <WiFi.h>
#include "ThingSpeak.h"
#include <SoftwareSerial.h>

const char* ssid     = "Livebox-08B0";
const char* password = "G79ji6dtEptVTPWmZP";

#include <SPI.h>               
#include <LoRa.h>
#define SCK     18   // GPIO18 -- SX127x's SCK
#define MISO    19   // GPIO19 -- SX127x's MISO
#define MOSI    23   // GPIO23 -- SX127x's MOSI
#define SS       5   // GPIO05 -- SX127x's CS
#define RST     15   // GPIO15 -- SX127x's RESET
#define DI0     25   // GPIO26 -- SX127x's IRQ(Interrupt Request)
#define freq    8685E5   
#define sf 9
#define sb 125E3  

typedef union 
{
 uint8_t frame[16]; // frames with bytes
  float  data[4];   // 4 floating point values
} pack_t ;  // packet type

WiFiClient  client;

unsigned long myChannelNumber = 1697980;
const char * myWriteAPIKey = "4K897XNNHTW7I4NO";

int rssi=0;
QueueHandle_t dqueue;  // queues for data packets

void onReceive(int packetSize) 
{
pack_t rdp;
Serial.println("received packet");
  if(packetSize==16)
  {
    int i=0;
    while (LoRa.available())  {  rdp.frame[i]=LoRa.read();i++; }
    rssi=LoRa.packetRssi(); 
    xQueueReset(dqueue); // to keep only the last element 
    xQueueSend(dqueue, &rdp, portMAX_DELAY); 
  }
}

void setup() {
  Serial.begin(9600); 
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
  dqueue = xQueueCreate(4,16); // queue for 4 data packets
  LoRa.onReceive(onReceive);  // register the receive callback
  LoRa.receive();  // put the radio into receive mode
}


void loop()   
{
  pack_t rdp;
  xQueueReceive(dqueue,rdp.frame,portMAX_DELAY); // default:portMAX_DELAY 
  Serial.printf("V:%2.2f,T:%2.2f,H:%2.2f,L:%2.2f\n",rdp.data[0],rdp.data[1],rdp.data[2],rdp.data[3]);
  Serial.printf("RSSI=%d\n",rssi);
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
  delay(15000);
}