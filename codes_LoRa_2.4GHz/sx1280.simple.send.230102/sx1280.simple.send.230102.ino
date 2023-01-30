
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

SX1280 radio = new Module(RADIO_CS_PIN, RADIO_DI0_PIN, RADIO_RST_PIN, RADIO_BUSY_PIN);

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
   // we got a packet, set the flag
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
delay(1500); 
radio.setDio1Action(setFlag);
}



void loop()
{

  Serial.println(millis());
  Serial.printf("[SX1280] Sending another packet :%d\n ", counter);
  transmissionState = radio.startTransmit((uint8_t *)&counter, 4);
  Serial.println(transmissionState);
  counter++;
  delay(1000);
 
}
