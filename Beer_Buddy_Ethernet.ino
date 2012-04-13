#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

#include <SoftwareSerial.h>
#include <BeerBuddyEthernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x03, 0x05 };
IPAddress ip(85,214,92,47);
char host[] = "http://beer-buddy.nl";
char received;
char state = 'I';
char* rfid;

BeerBuddyEthernet ethernet(mac, ip, host);

SoftwareSerial RFModule(2, 3);

void setup()
{
  Serial.begin(9600);
  RFModule.begin(9600);
  
  ethernet.initialize();
  ethernet.setOnline();
}

void loop()
{
  if ( state = 'I' )
  {
    // Receive
    if ( RFModule.available() )
    {
      received = (char)RFModule.read();
      Serial.println(received);
      lookForRfid(received);
      received = 0;
    }
    // Transmit
    if ( Serial.available() )
    {
      received = (char)Serial.read();
      RFModule.println(received);
      lookForRfid(received);
      received = 0;
    }
  }
  if ( state == 'R' )
  {
    if ( RFModule.available() )
    {
      received = (char)RFModule.read();
      rfid = ethernet.sendRFID(&received);
      state = 'I';
    }
    if ( Serial.available() )
    {
      received = (char)Serial.read();
      rfid = ethernet.sendRFID(&received);
      state = 'I';
    }
  }
}

void lookForRfid(char input)
{
  if ( input == 'R' )
  {
    ethernet.keepAlive();
    
    state = 'R';
  }
}

boolean getSerialValue()
{
  // Receive
  if ( RFModule.available() )
  {
    received = (char)RFModule.read();
    Serial.println(received);
    return true;
  }
  // Transmit
  if ( Serial.available() )
  {
    received = (char)Serial.read();
    RFModule.println(received);
    return true;
  }
  
  return false;
}
