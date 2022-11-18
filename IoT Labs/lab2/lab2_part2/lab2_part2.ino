#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define LEDPIN 15
int analogPin = A0;
#define SOCKET 8888
#define BUFFERLEN 255

char inBUFFER[BUFFERLEN];
char outBUFFER[BUFFERLEN];

WiFiUDP UDP;
const char ssid[] = "Nofal";
const char password[] = "nofal123";

void setup()
{
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(115200);
  Serial.print("Attempting to connect to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n");
  Serial.println("Successfully connected");
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());

  if ( UDP.begin(SOCKET))
    Serial.println("UDP connection is successful");



}
void loop() {
  int packetSize = 0;
  packetSize = UDP.parsePacket();

  if (packetSize)
  {

    UDP.read(inBUFFER, BUFFERLEN);
    inBUFFER[packetSize] = '\0';
    Serial.print("Received ");
    Serial.print(packetSize);
    Serial.println(" bytes");
    Serial.println("Contents:");
    Serial.println(inBUFFER);
//    Serial.print("From IP ");
//    Serial.println(UDP.remoteIP());
//    Serial.print("From port ");
//    Serial.println(UDP.remotePort());
    if (!strcmp(inBUFFER, "LEDON"))
    {
      digitalWrite(LEDPIN, HIGH);
    }
     if (!strcmp(inBUFFER, "LEDOFF"))
    {
      digitalWrite(LEDPIN, LOW);
    }

    //    UDP.beginPacket(UDP.remoteIP(),UDP.remotePort());
    ////    strcpy(outBUFFER, "This is a reply from Dr Nock's ESP8266 firmware");
    //    int value = analogRead(analogPin);
    //    sprintf(outBUFFER,"username ADC value is %d",value);
    //    UDP.write(outBUFFER);
    //    UDP.endPacket();
  }
}
