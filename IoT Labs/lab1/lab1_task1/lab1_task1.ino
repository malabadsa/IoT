#define LEDPIN 15
int analogPin = A0;

void setup ()
{
pinMode(LEDPIN, OUTPUT);
}

void loop()
{
int value = analogRead(analogPin);
digitalWrite(LEDPIN, HIGH) ;
delay (value+1 ) ;
digitalWrite(LEDPIN, LOW ) ;
delay(value+1 ) ;
}
// Set the LED pi n (15) to a l ogic l {··3 . 3VI
// wait for 0 .. 5 seconds
// Clear the LED pin (151 t o a l ogi c 0 {~OVI
// wait f or 0 . 5 s,econds
