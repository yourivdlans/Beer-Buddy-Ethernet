#include <SoftwareSerial.h>

SoftwareSerial RFModule(2, 3);

void setup()
{
  Serial.begin(9600);
  RFModule.begin(9600);
}

void loop()
{
  if (RFModule.available()) {
    Serial.print((char)RFModule.read());
  }
  if (Serial.available()) {
    RFModule.print((char)Serial.read());
  }
}
