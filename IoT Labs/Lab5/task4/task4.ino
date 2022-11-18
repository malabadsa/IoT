#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <rgb_lcd.h>        // include the grove RGB LCD library
#include <SimpleDHT.h>


/*********************************** Hardware defines ***********************************************
 *  
 ***************************************************************************************************/
#define LEDPIN      15
#define ANALOGUE  A0
#define DHTPIN 16

/******************************* Function prototypes ************************************************
 *  
 ***************************************************************************************************/
void MQTTconnect ( void );

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
SimpleDHT22 dht22;

static const char *fingerprint PROGMEM = "77 00 54 2D DA E7 D8 03 27 31 23 99 EB 27 DB CB A5 4C 57 18";

// create an instance of the Adafruit MQTT class. This requires the client, server, portm username and
// the Adafruit key
Adafruit_MQTT_Client MQTT(&client, ADASERVER, ADAPORT, ADAUSERNAME, ADAKEY);

int red = 0, green = 128, blue = 128;             // variables for the RGB backlight colour

/******************************** Feeds *************************************************************
 *  
 ***************************************************************************************************/
//Feeds we publish to

// Setup a feed called LED to subscibe to HIGH/LOW changes
Adafruit_MQTT_Subscribe LED = Adafruit_MQTT_Subscribe(&MQTT, ADAUSERNAME "/feeds/led");
Adafruit_MQTT_Publish POT = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/pot");
Adafruit_MQTT_Publish TEMP  = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/temp");
Adafruit_MQTT_Publish HUMID  = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/humid");



/********************************** Main*************************************************************
 *  
 ***************************************************************************************************/

int gettemphumid (float *temperature, float *humidity)
{
  if( dht22.read2(DHTPIN, temperature, humidity, NULL) != SimpleDHTErrSuccess )
  {
    Serial.print("Failed to read DHT sensor");
    delay(2000);
    *temperature = -999;
    *humidity = -999;
    return 0; //fall
  }

  Serial.print("Humidity: ");
  Serial.print(*humidity);
  Serial.println(" RH(%)");
  Serial.print("Temperature: ");
  Serial.print(*temperature);
  Serial.println(" *C");
  return 1; //success
}
 
void setup() 
{
  pinMode(LEDPIN,OUTPUT);
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

  LCD.print(WiFi.localIP());                    // update the display with the local (i.e. board) IP address

  
  MQTT.subscribe(&LED);                         // subscribe to the LED feed
  client.setFingerprint(fingerprint);
}
/************************************** Loop ********************************************************
 *  
 ***************************************************************************************************/
void loop() 
{  
  float temp, humid;
  gettemphumid(&temp,&humid);
  // This function call ensures that we have a connection to the Adafruit MQTT server. This will make
  // the first connection and automatically tries to reconnect if disconnected. However, if there are
  // three consecutive failled attempts, the code will deliberately reset the microcontroller via the 
  // watch dog timer via a forever loop.
  MQTTconnect();

  // an example of subscription code
  Adafruit_MQTT_Subscribe *subscription;                    // create a subscriber object instance
  while ( subscription = MQTT.readSubscription(5000) )      // Read a subscription and wait for max of 5 seconds.
  {                                                         // will return 1 on a subscription being read.
    if (subscription == &LED)                               // if the subscription we have receieved matches the one we are after
    {
      Serial.print("Recieved from the LED subscription:");  // print the subscription out
      Serial.println((char *)LED.lastread);                 // we have to cast the array of 8 bit values back to chars to print
      if(!strcmp((char *)LED.lastread,"HIGH"))
            digitalWrite(15, HIGH);
       if(!strcmp((char *)LED.lastread,"LOW"))
            digitalWrite(15, LOW);
 
      // ADD code here to compare (char *)LED.lastread against "HIGH" or "LOW". Refer to previous labs 
      // for how to use STRCMP. You should use two ifs rather than an if then else structure.
      // we could also convert characters received into integer variables via the use of atoi for passing values from the 
      // the broker to the ESP8266
      
     
    }
  }
 

  // Add publishing code here
//  if(!POT.publish(analogRead(ANALOGUE)))
//  Serial.println("The publishing failed");
//  else
//  Serial.println("The publishing succeeded");

  TEMP.publish(temp);
  HUMID.publish(humid);

  
  // keep alive code. If we are only subscribing, we need to ensure that we are connected. If we do not
  // get a response from a ping to the MQQT broker, disconnect such that on the next loop iteration, we reconnect.
  // Note that this is not needed if we publish to the broker every 10 seconds or less.
  // Compile time directives are used to add or remove this code to the sketch. Once you are publishing,
  // simply remove this code from the sketch by commenting out #define NOPUBLISH at the top of the sketch
  #ifdef NOPUBLISH
  if ( !MQTT.ping() ) 
  {
    MQTT.disconnect();
  }
  #endif
}
/******************************* MQTT connect *******************************************************
 *  
 ***************************************************************************************************/
void MQTTconnect ( void ) 
{
  unsigned char tries = 0;

  // Stop if already connected.
  if ( MQTT.connected() ) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ( MQTT.connect() != 0 )                                        // while we are 
  {     
       Serial.println("Will try to connect again in five seconds");   // inform user
       MQTT.disconnect();                                             // disconnect
       delay(5000);                                                   // wait 5 seconds
       tries++;
       if (tries == 3) 
       {
          Serial.println("problem with communication, forcing WDT for reset");
          while (1)
          {
            ;   // forever do nothing
          }
       }
  }
  
  Serial.println("MQTT succesfully connected!");
}
