#include <WiFi.h>
#include <WiFiUdp.h>
/* WiFi network name and password */
const char * ssid= "Livebox-08B0";
const char * pwd = "G79ji6dtEptVTPWmZP";
const char * udpAddress = "192.168.1.255";
const int udpPort = 1234;
//create UDP instance
WiFiUDP udp;

union
{
  uint8_t frame[16];
  float sensor[4];
  char mess[16];
} rdp,sdp;

void setup()
{
  Serial.begin(115200);
  //Connect to the WiFi network
  WiFi.begin(ssid, pwd);
    Serial.println("");
  // Wait for connection
  while (WiFi.status()!=WL_CONNECTED)
  {
      delay(500);Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //This initializes udp and transfer buffer
  udp.begin(udpPort);
}
void loop()
{
  memset(rdp.frame,0,16);
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  if(udp.read(rdp.frame,16)==16) // read packet from client
  {
    Serial.printf("\nRecv data:%f,%f,%f,%f\n",rdp.sensor[0],rdp.sensor[1],rdp.sensor[2],rdp.sensor[3]);
    //Wait for 1 second
    delay(1000);
    Serial.print("Send ACK packet ");
    udp.beginPacket(udpAddress, udpPort);
    strcpy(sdp.mess,"ACK data");
    udp.write(sdp.frame,16);
    udp.endPacket();  // send packet to client
  }
}
