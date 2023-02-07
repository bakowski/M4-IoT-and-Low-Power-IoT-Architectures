#include <WiFiManager.h> 
#include "ThingSpeak.h"

unsigned long myChannelNumber =1626377;   
const char *myWriteAPIKey="3IN09682SQX3PT4Z" ;
const char *myReadAPIKey="9JVTP8ZHVTB9G4TT" ;

WiFiClient  client;

void setup() {
    WiFi.mode(WIFI_STA); 
    Serial.begin(9600); 
    WiFiManager wm;
    //reset settings - wipe credentials for testing
    // wm.resetSettings();
    bool res;
    res = wm.autoConnect("ESP32AP",NULL); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }   
  ThingSpeak.begin(client); // connexion (TCP) du client au serveur
  delay(1000);
  Serial.println("ThingSpeak begin");
}


float temperature=0.0,humidity=0.0;
float tem,hum;

void loop() {
  Serial.println("Fields update");
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  delay(16000);
  temperature+=0.1;
  humidity+=0.2;
  tem =ThingSpeak.readFloatField(myChannelNumber,1);
// Note that if channel is private you need to add the read key
  Serial.print("Last temperature:"); 
  Serial.println(tem);
  delay(1000);
  hum =ThingSpeak.readFloatField(myChannelNumber,2,myReadAPIKey);
  Serial.print("Last humidity:"); 
  Serial.println(hum);
  delay(16000);    
}
