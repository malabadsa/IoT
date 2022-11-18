#include<RFID.h>
#include<SPI.h>
#include<rgb_lcd.h>
#define SS_PIN 0
#define RST_PIN 2
int cardpresent =0, lastcardpresent=0;
int count=0;
RFID myrfid(SS_PIN,RST_PIN);
int red=100;
int green=100;
int blue=100;;
rgb_lcd LCD;






void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial)
  {
    ;
  }
  SPI.begin();
  myrfid.init();
  LCD.begin (16,2);
  LCD.setRGB(red,green,blue);
  
 }



void loop() {
  // put your main code here, to run repeatedly:
int i;

cardpresent=myrfid.isCard();
if(cardpresent && !lastcardpresent)
{
  
     if(myrfid.readCardSerial())
    {
//      for(i=0;i<4;i++)
//          {
//           Serial.printf("%3d",myrfid.serNum[i]);
//           Serial.printf("\n");
//          }
         if(myrfid.serNum[0]==230&&myrfid.serNum[1]==237&&myrfid.serNum[2]==137&&myrfid.serNum[3]==161)
             
             { LCD.setCursor(0,0);
               LCD.printf("Aser Nassar");
               LCD.setRGB(0,255,0);
               delay(5000);
               LCD.setRGB(red,green,blue);
               LCD.clear();
               count=0;
              
          
          }
          else
          {
            LCD.setCursor(0,0);
            LCD.printf("Access Denied!");
            LCD.setRGB(255,0,0);
            count=count+1;
            LCD.setCursor(0,1);
            LCD.printf("#attempts=%d",count);
            delay(5000);
            LCD.setRGB(red,green,blue);
            LCD.clear();
            
          }
      }
      


  }
  lastcardpresent=cardpresent;
  myrfid.halt();
}
