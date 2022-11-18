#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <rgb_lcd.h>        // include the grove RGB LCD library
#include <WiFiUdp.h>


/*********************************** Hardware defines ***********************************************
 *  
 ***************************************************************************************************/
#define LEDPIN      15
#define ANALOGUE  A0
#define DHTPIN 16

/******************************* Function prototypes ************************************************
 *  
 ***************************************************************************************************/

/*********************************** Network defines ************************************************
 *  
 ***************************************************************************************************/
#define ssid          "Nofal"    // your hotspot SSID or name
#define password      "nofal123"    // your hotspot password

/*********************************** Adafruit IO defines*********************************************
 *  
 ***************************************************************************************************/
//#define NOPUBLISH      // comment this out once publishing at less than 10 second intervals

#define ADASERVER     "io.adafruit.com"     // do not change this
#define ADAPORT       8883                  // do not change this 
#define ADAUSERNAME   "200238391"               // ADD YOUR username here between the qoutation marks
#define ADAKEY        "aio_fTLl70kUbK78v4oOhXxBRQTT6vH6" // ADD YOUR Adafruit key here betwwen marks

/******************************** Global instances / variables***************************************
 *  
 ***************************************************************************************************/
WiFiClientSecure client;    // create a class instance for the MQTT server
rgb_lcd LCD;          // create a class instance of the rgb_lcd class
WiFiUDP UDP;

NTPClient ntpTIME( UDP, "pool.ntp.org", 0);

// create an instance of the Adafruit MQTT class. This requires the client, server, portm username and
// the Adafruit key

int red = 0, green = 128, blue = 128;             // variables for the RGB backlight colour

/******************************** Feeds *************************************************************
 *  
 ***************************************************************************************************/
//Feeds we publish to

// Setup a feed called LED to subscibe to HIGH/LOW changes


/********************************** Main*************************************************************
 *  
 ***************************************************************************************************/

 
void setup() 
{
  Serial.begin(115200);                         // open a serial port at 115,200 baud
  while(!Serial)                                // wait for serial peripheral to initialise
  {
    ;
  }
  delay(10);                                    // additional delay before attempting to use the serial peripheral
  Serial.print("Attempting to connect to ");    // Inform of us connecting
  Serial.print(ssid);                           // print the ssid over serial
  
  WiFi.begin(ssid, password);                   // attemp to connect to the access point SSID with the password
  
  while (WiFi.status() != WL_CONNECTED)         // whilst we are not connected
  {
    delay(500);                                 // wait for 0.5 seconds (500ms)
    Serial.print(".");                          // print a .
  }
  Serial.print("\n");                           // print a new line
  
  Serial.println("Succesfully connected");      // let us now that we have now connected to the access point
  Serial.print("Mac address: ");                // print the MAC address
  Serial.println(WiFi.macAddress());            // note that the arduino println function will print all six mac bytes for us
  Serial.print("IP:  ");                        // print the IP address
  Serial.println(WiFi.localIP());               // In the same way, the println function prints all four IP address bytes
  Serial.print("Subnet masks: ");               // Print the subnet mask
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");                    // print the gateway IP address
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");                        // print the DNS IP address
  Serial.println(WiFi.dnsIP());
   
  LCD.begin(16, 2);                             // initialise the LCD
  LCD.setRGB(red, green, blue);                 // set the background colour
  
  LCD.print(WiFi.RSSI());                    // update the display with the local (i.e. board) IP address
  ntpTIME.begin();
}
/************************************** Loop ********************************************************
 *  
 ***************************************************************************************************/
void loop() 
{  
  
  LCD.begin(16, 2);                             // initialise the LCD
  LCD.setRGB(red, green, blue);                 // set the background colour

  LCD.print(WiFi.RSSI());
  ntpTIME.update();
  
  Serial.print(ntpTIME.getHours());  
       Serial.print(":");
  Serial.print(ntpTIME.getMinutes()); 
  Serial.print(":");      
  Serial.print(ntpTIME.getSeconds()); 
  Serial.print("\n");                   
}
