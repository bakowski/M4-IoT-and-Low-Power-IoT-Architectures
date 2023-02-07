#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup() {
    WiFi.mode(WIFI_STA); // set mode to STA+AP
    Serial.begin(115200); 
    WiFiManager wm;
    //reset settings - wipe credentials for testing
    wm.resetSettings();  // à tester , puis à commenter
    // Automatically connect using saved credentials,
    bool res;
    res = wm.autoConnect("ESP32C3AP",NULL); // no password
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}

void loop() 
{ 
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  delay(4000);
  }
