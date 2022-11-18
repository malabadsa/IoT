#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define SOCKET 8888
WiFiUDP UDP;
const char ssid[] = "Nofal";
const char password[] = "nofal123";

void setup()
{
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
  Serial.println(WiFi.dnsIP());

// if( UDP.begin(SOCKET))
// Serial.println("UDP connection is successful");
//  

 
}
void loop(){
}
