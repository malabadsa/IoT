
void setup() 
{
  Serial.begin(9600);
  while (!Serial)
  {
  
  }
}
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
void loop() {
    if(checkbutton() == 1)
    {
      Serial.println("user name is nofal");
    }
    
    
  // put your main code here, to run repeatedly:

}
