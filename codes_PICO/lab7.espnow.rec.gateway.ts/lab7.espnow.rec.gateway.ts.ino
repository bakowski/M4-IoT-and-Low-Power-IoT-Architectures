#include <esp_now.h>
#include <WiFi.h>
#include "ThingSpeak.h" 

WiFiClient  client;
QueueHandle_t queue;

const char * ssid= "Livebox-08B0";
const char * pwd = "G79ji6dtEptVTPWmZP";
//static const char* ssid     = "PhoneAP";
//static const char* password = "smartcomputerlab";

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *buff, int len) 
{
  union 
  {
  uint8_t  frame[16];
  int sensor[4];
  } pack;
  
  memcpy(pack.frame, buff,len);
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Int: ");
  Serial.println(pack.sensor[0]);
  Serial.print("Int: ");
  Serial.println(pack.sensor[1]);
  Serial.println();
  xQueueReset(queue);
  xQueueSend(queue, pack.frame, 0);
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);Serial.println();
  // Set device as a Wi-Fi Station for WiFi and AP for ESP-NOW
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(); Serial.println("WiFi connected");
  ThingSpeak.begin(client);  // Initialize ThingSpeak
   queue = xQueueCreate(10,16);  // 10 slots for 4 integer
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  union 
  {
  uint8_t  frame[16];
  int sensor[4];
  } rpack;
  
  xQueueReceive(queue, rpack.frame, portMAX_DELAY);
  ThingSpeak.setField(1, rpack.sensor[0]);
  ThingSpeak.setField(2, rpack.sensor[1]);
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(1626377,  "3IN09682SQX3PT4Z" );
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(16000);
}
