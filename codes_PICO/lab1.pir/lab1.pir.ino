#define PIR 6
bool MOTION_DETECTED = false;

void pinChanged() 
{
  MOTION_DETECTED = true;
}
void setup() 
{
  Serial.begin(9600);
  attachInterrupt(PIR, pinChanged, RISING);
}

int counter=0;

void loop()
{
  int i=0;
  if(MOTION_DETECTED)
  {
    Serial.println("Motion detected.");
    delay(1000);counter++;
    MOTION_DETECTED = false;
    Serial.println(counter);
  }
}
