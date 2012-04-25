#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>

#include <SoftwareSerial.h>
#include <BeerBuddyEthernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x03, 0x05 };
IPAddress remote_ip(85,214,92,47);
IPAddress local_ip(145,52,201,100);
char host[] = "http://beer-buddy.nl";
char received;
// I = Idle, R = RFID, A = Alcohol
char state = 'I';
char rfid[12];
char* user;
int i;
int y;

BeerBuddyEthernet ethernet(mac, remote_ip, host);

SoftwareSerial RFModule(2, 3);

void setup()
{
  Serial.begin(9600);
  RFModule.begin(9600);
  
  ethernet.setLocalIp(local_ip);
  ethernet.initialize();
  ethernet.setOnline();
}

void loop()
{
  if ( state == 'I' )
  {
    i = 0;
    
    if ( Serial.available() == 1 )
    {
      while ( Serial.available() )
      {
        received = (char)Serial.read();
        
        if ( i == 0 && received == 'R' )
        {
          // Check if server is online
          Serial.println("1");
          state = 'R';
        }
        
        ++i;
      }
    }
  }
  
  if ( state == 'R' )
  {
    i = 0;
    y = 0;
    
    if ( Serial.available() == 12 )
    {
      while ( Serial.available() )
      {
        received = (char)Serial.read();
        
        Serial.print("I: ");
        Serial.print(i);
        Serial.print(" - ");
        Serial.print("Received: ");
        Serial.print(received);
        
        if ( i != 0 && i <= 12 )
        {
          Serial.print(" <= added to rfid[");
          Serial.print(y);
          Serial.print("]");
          
          rfid[y] = received;
          ++y;
          
          if ( i == 12 )
          {
            rfid[y] = '\0';
            
            //Serial.print("Y: ");
            //Serial.print(y);
            //Serial.println();
          }
        }
        
        Serial.println();
        
        ++i;
      }
      
      user = ethernet.sendRFID(rfid);
      
      Serial.println(user);
      
      state = 'I';
    }
  }
}
