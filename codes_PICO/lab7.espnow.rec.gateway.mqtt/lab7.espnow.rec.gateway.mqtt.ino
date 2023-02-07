#include <esp_now.h>
#include <WiFi.h>
#include <MQTT.h>

WiFiClient  net;
MQTTClient client;

QueueHandle_t queue;

const char * ssid= "Livebox-08B0";
const char * pwd = "G79ji6dtEptVTPWmZP";
//static const char* ssid     = "PhoneAP";
//static const char* password = "smartcomputerlab";

const char* mqttServer = "broker.emqx.io";


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

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  // send LoRa message depending on topic
}


void connect() {
  char cbuff[128];
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
       delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("IoT.GW1")) {
    Serial.print("."); delay(1000);
  }
  Serial.println("\nIoT.GW1 - connected!");
  client.subscribe("/esp32_GW1/Test");
  delay(1000);
  
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
  delay(200);
  Serial.println(); 
  Serial.println("WiFi connected");
  queue = xQueueCreate(10,16);  // 10 slots for 4 integer
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  client.begin(mqttServer, net);
  client.onMessage(messageReceived);
  connect();
}

unsigned long lastMillis = 0;

void loop() 
{
  union 
  {
  uint8_t  frame[32];
  int sensor[4];
  } rpack;

  char buff[64];
  
 // client.loop();
  delay(10);  // <- fixes some issues with WiFi stability
  xQueueReceive(queue, rpack.frame, portMAX_DELAY);
  Serial.print("\nconnecting...");
  if (!client.connected()) { connect(); }
    if (millis() - lastMillis > 5000) {   // publish a message every 5 seconds
    lastMillis = millis();
    sprintf(buff,"Temp:%d, Humi:%d\n",rpack.sensor[0],rpack.sensor[1]);   
    Serial.println(buff);
    client.publish("/esp32_GW1/Test",buff);
    Serial.println("in the loop");
    delay(2000);
    }
}
