
void setup()
{
Serial.begin(9600);
while (!Serial)
{
  
}
// setup a Board rate of 9600 bits per second
// whi. le the serial port i.s not ready
// do nothing
// print some lines notifying the user of note that pr intln pr ints a new line
Serial.println("Hello World I This is STUDENT NAME' s ESP8Z66");
Serial.println( "This is how a constant string can be printed with a new line .. . . ");
}
int num = 0; // a v ari abl e used to store numhers recei ved from the PC
void loop()
{
if ( Serial. available() > 0 )
{
num = Serial.parseInt();
}
if (Serial.read() == '\n' )
{
Serial.print( "num's value is:");
Serial.print(num,DEC);
}
}
// i. f we have data recei. ved from the PC
// parse (fi.nd and convert) an integer m.rnber i.f present
// in the ser ial buffer .
// i f we have r eached the end of a line sent Ci. e . new line/ enter pr essed)
// print the str ing passed
// print the number held in num as text in base 10 ( decimal)
// add more l i.nes here to di.spl ay the number i.n bi.nary and hexadeci.mal number systems
