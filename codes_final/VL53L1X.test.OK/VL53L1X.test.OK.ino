#include "Adafruit_VL53L1X.h"

#define IRQ_PIN 2
#define XSHUT_PIN 3

Adafruit_VL53L1X vl53 = Adafruit_VL53L1X(XSHUT_PIN, IRQ_PIN);

int16_t get_distance()
{
  int16_t distance;
  
  if (vl53.dataReady()) {
    // new measurement for the taking!
    distance = vl53.distance();
    if (distance == -1) {
      // something went wrong!
//      Serial.print(F("Couldn't get distance: "));
//      Serial.println(vl53.vl_status);
      return vl53.vl_status;
    }
    // data is read out, time for another reading!
    vl53.clearInterrupt();
  }
  return distance;
}

uint16_t DistanceMode=2;

void setup() 
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
  delay(500);
  
  Wire.begin(28,29);

  Serial.begin(9600);
  Serial.println("VL53L1X Test");
  if (! vl53.begin(0x29, &Wire)) {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));
  Serial.print(F("Sensor ID: 0x"));
  Serial.println(vl53.sensorID(), HEX);

  if (! vl53.startRanging()) {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(vl53.vl_status);
    while (1)       delay(10);
  }
  Serial.println(F("Ranging started"));

  // Valid timing budgets: 15, 20, 33, 50, 100, 200 and 500ms!
  //vl53.SetDistanceMode(DistanceMode);
  vl53.setTimingBudget(50);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(vl53.getTimingBudget());


  /*
  vl.VL53L1X_SetDistanceThreshold(100, 300, 3, 1);
  vl.VL53L1X_SetInterruptPolarity(0);
  */
}

void loop() 
{
  int16_t distance;
//
//  if (vl53.dataReady()) {
//    // new measurement for the taking!
//    distance = vl53.distance();
//    if (distance == -1) {
//      // something went wrong!
//      Serial.print(F("Couldn't get distance: "));
//      Serial.println(vl53.vl_status);
//      return;
//    }
//    Serial.print(F("Distance: "));
//    Serial.print(distance);
//    Serial.println(" mm");
//
//    // data is read out, time for another reading!
//    vl53.clearInterrupt();
//  }
  distance=get_distance();
  if(distance>0)
  {
    Serial.print(F("Distance: "));
    Serial.print(distance);
    Serial.println(" mm");
  }
  delay(2000);
}
