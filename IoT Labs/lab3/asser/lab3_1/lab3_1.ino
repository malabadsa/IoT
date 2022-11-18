
#include<RFID.h>
#include<SPI.h>
#include<rgb_lcd.h>
int timespressed=0;
boolean toggle=false;
int current_millis;
int task1LC=0;
int task2LC=0;
int task3LC=0;
#define TASK1L 5
#define TASK2L 500
#define TASK3L 1000

int red=100;
int green=100;
int blue=100;;
rgb_lcd LCD;
int checkbutton () {
    pinMode(15,INPUT);

  static int current;
  current = digitalRead(15);
  static int last;
  if(current && !last){
  delay(5);
  if(digitalRead(15)==HIGH){
    pinMode(15,OUTPUT);
    last = current;
    return 1;
  }
  }
    pinMode(15,OUTPUT);
    last = current;
    return 0;
}
void task1(void)
{
  if(checkbutton()==1)
  {
    timespressed=timespressed+1;
  }
  }
void task2 (void)
{
  toggle = !toggle;
  digitalWrite(15,toggle);
  
  }
  void task3 (void)
  {
     LCD.setCursor(0,1);
     LCD.printf("#presses=%d",timespressed);
    
    }
void setup() {
  pinMode(15,OUTPUT);
    SPI.begin();

  LCD.begin (16,2);
  LCD.setRGB(red,green,blue);
  

}

void loop() {
  // put your main code here, to run repeatedly:
 current_millis=millis();

 if ( (current_millis - task1LC) >= TASK1L )
{
  task1();
  task1LC = current_millis;
}
 
 if ( (current_millis - task2LC) >= TASK2L )
{
  task2();
  task2LC = current_millis;
}

  if ( (current_millis - task3LC) >= TASK3L )
{
  task3();
  task3LC = current_millis;
} 
  

}
