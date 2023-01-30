
#include <RadioLib.h>
#include <Wire.h>

// modem connections: SPI +

#define RADIO_SCLK_PIN              5
#define RADIO_MISO_PIN              19
#define RADIO_MOSI_PIN              27
#define RADIO_CS_PIN                18
#define RADIO_DI0_PIN               26
#define RADIO_RST_PIN               23
#define RADIO_DIO1_PIN              33
#define RADIO_BUSY_PIN              32

#define I2C_SDA              12
#define I2C_SCL              14

SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DIO1_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

int transmissionState = 0;
volatile bool receivedFlag = false;
volatile bool enableInterrupt = true;

uint32_t counter = 0;

void setFlag(void)
{
    // check if the interrupt is enabled
    if (!enableInterrupt) {
        return;
    }
    // we sent a packet, set the flag
    receivedFlag = true;

}

void setup() {
  Serial.begin(115200);
  Serial.println("initBoard");
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(1500); 
  Serial.print("[SX1280] Initializing ... ");
  delay(1500); 
  int state = radio.begin();
      if (state == 0) {
        Serial.println("success!");
    } else {
        Serial.print("failed, code ");
        Serial.println(state);
        while (true);
    }
 radio.setDio1Action(setFlag);
 Serial.print("[SX1280] Starting to listen ... ");
    state = radio.startReceive();
    if (state == 0) {
        Serial.println("success!");
    } else {
        Serial.print("failed, code ");
        Serial.println(state);
        while (true);
    }
  delay(2000);
  enableInterrupt=true;
}


void loop()
{
  if (receivedFlag) 
  {
        // disable the interrupt service routine while
        enableInterrupt = false;
        // reset flag
        receivedFlag = false;
        Serial.println("[SX1280] Receiving packet");
        counter=0;
        int state = radio.readData((uint8_t *)&counter, 4);
        Serial.println(state);
        Serial.println(radio.getRSSI());
        Serial.println(counter);
        radio.startReceive();
        enableInterrupt = true;
  }
  delay(100);
}
