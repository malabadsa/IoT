#include <RFID.h>
#include <SPI.h>
#define SS_PIN 0
#define RST_PIN 2
int cardpresent = 0, lastcardpresent = 0;
RFID myrfid(SS_PIN,RST_PIN);
void setup() 
{
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
  SPI.begin();
  myrfid.init();
}

void loop() 
{
  int i;
  cardpresent = myrfid.isCard();
  if( cardpresent && !lastcardpresent)
  {
    if(myrfid.readCardSerial())
    {
      for (i = 0; i<4; i++){
        Serial.printf("%3d ", myrfid.serNum[i]);
      }
      Serial.printf("\n");
      }
    }
    lastcardpresent = cardpresent;
    myrfid.halt();
 }
