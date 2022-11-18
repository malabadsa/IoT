#include<RFID.h>
#include<SPI.h>
#include<rgb_lcd.h>
#include "SimpleDHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define SS_PIN 0
#define RST_PIN 2
#define LEDPIN 15

//DEFINE MQTT Parameters
#define ssid          "Md-pc"    // your hotspot SSID or name
#define password      "md123123"    // your hotspot password
#define ADASERVER     "io.adafruit.com"     // do not change this
#define ADAPORT       1883                  // do not change this 
#define ADAUSERNAME   "MoAlabadsa"               // ADD YOUR username here between the qoutation marks
#define ADAKEY        "aio_ShOj11GIfUYmDGDDrW3qQ6nH4MWs" // ADD YOUR Adafruit key here betwwen marks


int cardpresent = 0;
int lastcardpresent = 0;

int analogPin = A0;

int current_millis;
int task1LC = 0;
#define TASK1L 100
#define TIMEOUTVAL 250
#define LINES 8
#define LEN 150
int i=0;

int flag = 0;

int sensor_val;
float lower_temp_limit = 36.5;
float upper_temp_limit = 38;
int base_temp = 25;
int high_temp = 45;
char wifilocalip[16];
char inputbuf[LINES][LEN] = {};

struct value {
  float temp;
  char rfid_serial [12];
};
struct value arr_value[10];
char loc[] = "Main Office";
rgb_lcd LCD;
RFID myrfid(SS_PIN, RST_PIN);
WiFiClient client;     // create a class instance for the MQTT server
WiFiClient MQTT_Client;    // create a class instance for the MQTT server //////
WiFiServer SERVER(80);

Adafruit_MQTT_Client MQTT(&MQTT_Client, ADASERVER, ADAPORT, ADAUSERNAME, ADAKEY);

Adafruit_MQTT_Publish LOCATION = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/location");
Adafruit_MQTT_Publish TEMP = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/temp");
Adafruit_MQTT_Publish IDCARD = Adafruit_MQTT_Publish(&MQTT, ADAUSERNAME "/feeds/idcard");
Adafruit_MQTT_Subscribe LOWLIMIT = Adafruit_MQTT_Subscribe(&MQTT, ADAUSERNAME "/feeds/lowlimit");
Adafruit_MQTT_Subscribe UPLIMIT = Adafruit_MQTT_Subscribe(&MQTT, ADAUSERNAME "/feeds/uplimit");

#define BUFFERLEN 255
char inBUFFER[BUFFERLEN];
char outBUFFER[BUFFERLEN];

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
    if (tries == 4)
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

void servepage ( void )
{ int count;

  String reply;
  char buffer [50];
  reply += "<!DOCTYPE HTML>";
  reply += "<html>";
  reply += "<head>";

  reply += "<title>Temperature Monitor</title>";
  reply += "</head>";
  reply += "<body>";
  reply += "<h1>Temperature Monitor</h1>";

  reply += "<br><a href=""/?increaseup"">Increase Up Limit </a>";
  reply += "<br><a href=""/?decreaseup"">Decrease Up Limit </a>";
  reply += "<br><a href=""/?increaselow"">Increase Low Limit </a>";
  reply += "<br><a href=""/?decreaselow"">Decrease Low Limit </a>";
  reply += "<br>";

  for (count = 0; count < 10; count++)
  {

    sprintf(buffer, "The temperature value is: %.2f", arr_value[count].temp);
    reply += buffer;
    reply += "<br>";
    sprintf(buffer, "Employee ID is:%s", arr_value[count].rfid_serial);
    reply += buffer;
    reply += "<br>";
    reply += "<br>";
    reply += "</body>";
    reply += "</html>";
  }
  client.printf("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %u\r\n\r\n%s", reply.length(), reply.c_str());
}


//************************************************************************************************************//
void respond(void) {

  servepage();
}

//************************************************************************************************************//
void setup() {

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

  Serial.println("Server is now running");

  SPI.begin();
  myrfid.init();
  LCD.begin (16, 2);
  LCD.setRGB(200, 200, 200);
  pinMode(LEDPIN, OUTPUT);

  LCD.setCursor(0, 1);
  char wifilocalip[16];
  strcpy(wifilocalip, WiFi.localIP().toString().c_str());
  LCD.printf(wifilocalip);
  SERVER.begin();

  MQTT.subscribe(&LOWLIMIT);
  MQTT.subscribe(&UPLIMIT);

}

//************************************************************************************************************//
int checkRFID(void) {

  cardpresent = myrfid.isCard();
  if (cardpresent && !lastcardpresent)
  {

    if (myrfid.readCardSerial())
    {
      if(i<10)
      {
      sensor_val = analogRead(analogPin);
      arr_value[i].temp = (sensor_val * ((high_temp - base_temp) / 1024.0)) + base_temp;
      sprintf(arr_value[i].rfid_serial, "%d%d%d%d", myrfid.serNum[0], myrfid.serNum[1], myrfid.serNum[2], myrfid.serNum[3]);

      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.printf("%.2f", arr_value[i].temp);
      //

      delay(2000);
      LCD.setRGB(200, 200, 200);
      LCD.clear();
      if (arr_value[i].temp >= lower_temp_limit && arr_value[i].temp <= upper_temp_limit)
      {

        digitalWrite(15, HIGH);
        delay(1000);
        digitalWrite(15, LOW);

      }
      else
      { LCD.clear();
        LCD.setCursor(0, 0);
        LCD.printf("Go Home !");
        delay(1000);
        LCD.clear();
         
      }
      i=i+1;
      }
      
     else
     {
        i=0;
     }
     
     }

      flag = 1;
    }

  else
  {
    LCD.setCursor(0, 0);
    LCD.printf("SHOW ID!");
    flag = 0;
  }
  lastcardpresent = cardpresent;
  myrfid.halt();
}

//************************************************************************************************************//
void loop() {

  MQTTconnect();
  int timecnt = 0; ///
  int p; ////
  //
  Adafruit_MQTT_Subscribe *subscription;

  while ( subscription = MQTT.readSubscription(500) )      // Read a subscription and wait for max of 5 seconds.
  {
    // will return 1 on a subscription being read.
    if (subscription == &LOWLIMIT)                               // if the subscription we have receieved matches the one we are after
    {  
      lower_temp_limit = atof((char*)LOWLIMIT.lastread);
    }
    if (subscription == &UPLIMIT)                               // if the subscription we have receieved matches the one we are after
    {
      upper_temp_limit = atof((char*)UPLIMIT.lastread);
    }
  }
  current_millis = millis();

  if ( (current_millis - task1LC) >= TASK1L )
  {
    checkRFID();
    if (flag)

    {
        
   TEMP.publish(arr_value[i-1].temp);
   IDCARD.publish(arr_value[i-1].rfid_serial);
      LOCATION.publish(loc);
    }
    task1LC = current_millis;
  }

  client = SERVER.available();

  if ( client) {

    while (!client.available())
    {
      delay(1);
      timecnt++;
      if (timecnt >= TIMEOUTVAL)
      {
        return;
      }
    }

    client.readStringUntil('\r').toCharArray( &inputbuf[p][0], LEN);

    if (!(strcmp(inputbuf[0], "GET /?increaseup HTTP/1.1")))
    { upper_temp_limit = upper_temp_limit + 1;

    }
    else if (!(strcmp(inputbuf[0], "GET /?decreasecup HTTP/1.1")))
    { upper_temp_limit = upper_temp_limit - 1;

    }
    else if (!(strcmp(inputbuf[0], "GET /?increaselow HTTP/1.1")))
    { lower_temp_limit = lower_temp_limit + 1;

    }
    else if (!(strcmp(inputbuf[0],  "GET /?decreaselow HTTP/1.1")))
    { lower_temp_limit = lower_temp_limit - 1;

    }
    client.flush();
    servepage();
    client.stop();


#ifdef NOPUBLISH
    if ( !MQTT.ping() )
    {
      MQTT.disconnect();
    }
#endif
  }
}

//************************************************************************************************************//
