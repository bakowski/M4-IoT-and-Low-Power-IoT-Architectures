#include <esp_now.h>
#include <WiFi.h>

union 
  {
  uint8_t  frame[16];
  int sensor[4];
  } pack;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *buff, int len) 
{
  memcpy(pack.frame, buff,len);
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Int: ");
  Serial.println(pack.sensor[0]);
  Serial.print("Int: ");
  Serial.println(pack.sensor[1]);
  Serial.println();
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
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
}
