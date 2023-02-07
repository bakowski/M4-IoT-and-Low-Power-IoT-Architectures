#include <WiFi.h>
#include <WiFiUdp.h>
/* WiFi network name and password */
const char * ssid= "Livebox-08B0";
const char * pwd = "G79ji6dtEptVTPWmZP";
const char * udpAddress = "192.168.1.255";  // on this network
const int udpPort = 1234;
//create UDP instance
WiFiUDP udp;

union
{
  uint8_t frame[16];
  float sensor[4];
  char mess[16];
} sdp, rdp; // send and receive packets

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pwd);
  Serial.println("");
  // Wait for connection
  while (WiFi.status()!=WL_CONNECTED){delay(500);Serial.print(".");}
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
  Serial.println("Send unicast/broadcast packet ");
  udp.beginPacket(udpAddress, udpPort);
  sdp.sensor[0]=0.1;sdp.sensor[1]=0.2;sdp.sensor[2]=0.3;sdp.sensor[3]=0.4;
  udp.write(sdp.frame, 16);
  udp.endPacket();
  memset(rdp.frame,0,16);
  //processing incoming packet, must be called before reading the buffer
  udp.parsePacket();
  //receive response from server, it will be ACK
  if(udp.read(rdp.frame,16)==16)
  {
    Serial.print("Received from server: "); Serial.println(rdp.mess);
  }
  //Wait for 2 seconds
  delay(2000);
}
