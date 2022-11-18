#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServerSecure.h>
#include <WiFiUdp.h>
#include<rgb_lcd.h>
#include <SimpleDHT.h>

#define DHTPIN 16

void servepage ( void );
void servepagewithtemp ( void );
BearSSL::ESP8266WebServerSecure SERVER(443);
rgb_lcd LCD;
int red=100;
int green=100;
int blue=100;

#define SOCKET 8888
WiFiUDP UDP;
SimpleDHT22 dht22;
const char ssid[] = "Nofal";
const char password[] = "nofal123";
const char domainname [] = "200238391";

static const char cert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIC8jCCAlugAwIBAgIUYOIOrtubcFsmeN3TrSfpvaJoMHAwDQYJKoZIhvcNAQEL
BQAwfjELMAkGA1UEBhMCVUsxFjAUBgNVBAgMDVdFU1QgTUlETEFORFMxEzARBgNV
BAcMCkJJUk1JTkdIQU0xGjAYBgNVBAoMEUFzdG9uIFVuaXZlcnNpdHkgMQ0wCwYD
VQQLDARFRVBFMRcwFQYDVQQDDA51c2VybmFtZS5sb2NhbDAeFw0yMDAyMTUxOTQ5
MzZaFw00NzA3MDMxOTQ5MzZaMH4xCzAJBgNVBAYTAlVLMRYwFAYDVQQIDA1XRVNU
IE1JRExBTkRTMRMwEQYDVQQHDApCSVJNSU5HSEFNMRowGAYDVQQKDBFBc3RvbiBV
bml2ZXJzaXR5IDENMAsGA1UECwwERUVQRTEXMBUGA1UEAwwOdXNlcm5hbWUubG9j
YWwwgZ8wDQYJKoZIhvcNAQEBBQADgY0AMIGJAoGBAKZPMudaME7JDhWSUhH+ZfOQ
dRZ6Pa4I1Xt0RJCb9F9PUX4vU7cyVWufoYpBlyOxzPyn1kMAzBmEQ0vAwXG0m7PU
urft8ZB/ZZallIfscgEI8HDcOVdwt/uEbfsONp7R4BAGKGhijx+niMgWLL3aoIF1
Tj7AqzjU85e/v2kPrdR5AgMBAAGjbTBrMB0GA1UdDgQWBBTAz79RjP3Mbs4Y32Ga
9p44Vycp+zAfBgNVHSMEGDAWgBTAz79RjP3Mbs4Y32Ga9p44Vycp+zAPBgNVHRMB
Af8EBTADAQH/MBgGA1UdEQQRMA+CDXVzZXJuYW1lLmxvY2EwDQYJKoZIhvcNAQEL
BQADgYEABmkelyDzfZRcqVGM8edRJM5dAQvBlHnjnsMPhvhgkG6MUisBa+vtVhKC
cOE4gAFvv8/hCaWIQwtFdWNfyle5meRX/Tj4L9N1aHASyPfTBcqKTGpjq6GiqtmL
rTINar7hi15E+adMVRRg/1VNfnBXKxh6IYch5hgeHj5TRgGwgAs=
-----END CERTIFICATE-----
)EOF";

static const char key[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
MIICdwIBADANBgkqhkiG9w0BAQEFAASCAmEwggJdAgEAAoGBAKZPMudaME7JDhWS
UhH+ZfOQdRZ6Pa4I1Xt0RJCb9F9PUX4vU7cyVWufoYpBlyOxzPyn1kMAzBmEQ0vA
wXG0m7PUurft8ZB/ZZallIfscgEI8HDcOVdwt/uEbfsONp7R4BAGKGhijx+niMgW
LL3aoIF1Tj7AqzjU85e/v2kPrdR5AgMBAAECgYBgNFpW+JYPTUDne6AcJpSlY8BH
w2jgvt13r9dl68FeTQzwOMJtrCE7w7j3uF+M13KkCRbp5ZErhZZEQPnmI7sZSkal
3TIWmLQq9g9mIscdIjc6rsfWJ7DAdpVgdsClS3sHPxv3D3RTy6Z40vzPgb2977/y
26pUHHJVEG27dqthbQJBANJPu7fm8EOUcvg1/wwnp3p+dCHEmZ7snQlUerMGbw3+
fSJTqFhrwEXYhfzj2VQUqZyaDBM+tZ9vUQU+BreM+fcCQQDKcFUVeUh6PzJJ1uSJ
F3gabOc1IoIPyotQ9RWAU70vaj/aAuIXJd6PsUt+0DucYfcg1G5YteUqJGl/5Mpv
CokPAkADNLHw2LVa4l1qSTBtGAGmjVzp0txgnsy6Aq6oIfX5aaKwrkPHrUTOC8Hn
G/YJIROAzpxWgsMz/fdnNA3YKG77AkEAiA7NsJwOMVNuKiCLAvTKHQCauKSTw6c+
0U+XfuNJIKgJeC495I7oMa1Yb0fm+KkDHoaID4lZF2TXn0SXJeBv0wJBAKhRejgb
R/KkoohDnWQCNkI9TOMPtTohGjoqMakVcaFHB+SOYhKi7iI0+coxNv/1dMydaE5X
xkjlush/Q6zlliA=
-----END PRIVATE KEY-----
)EOF";
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
void respond(void) {
  servepagewithtemp();
}

void setup()
{  pinMode(15,OUTPUT);
  LCD.begin (16,2);
  LCD.setRGB(red,green,blue);
  
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

  LCD.setCursor(0,0);
  LCD.print(WiFi.localIP());
  LCD.setCursor(0,1);
  LCD.print(WiFi.dnsIP());

// if( UDP.begin(SOCKET))
// Serial.println("UDP connection is successful");
//  

 if (MDNS.begin(domainname)){
  Serial.print("Access your server at https://");
  Serial.print(domainname);
  Serial.print(".local");
 }
 SERVER.getServer().setRSACert(new BearSSL::X509List(cert), new BearSSL::PrivateKey(key));
 SERVER.on("/", respond);
 SERVER.begin();
 Serial.println("Server is now running");
}
void loop(){
  SERVER.handleClient();
  MDNS.update();
  
}

void servepage ( void )
{
    String reply;

    reply += "<!DOCTYPE HTML>";
    reply += "<html>";
    reply += "<head>";
    reply += "<title>Dr Nock's sensor</title>";
    reply += "</head>";
    reply += "<body>";
    reply += "<h1>This is 200238391's wireless sensor node</h1>";

    reply += "You can add content here to represent various system parameters etc";
    reply += "</body>";
    reply += "</html>";
    
    SERVER.send(200, "text/html", reply);
}

void servepagewithtemp ( void )
{
    float temp, humid;
    gettemphumid(&temp,&humid);
    String reply;
    char buffer [50];
    reply += "<!DOCTYPE HTML>";
    reply += "<html>";
    reply += "<head>";
    reply += "<title>Dr Nock's sensor</title>";
    reply += "</head>";
    reply += "<body>";
    reply += "<h1>This is 200238391's wireless sensor node</h1>";

    //reply += "You can add content here to represent various system parameters etc";
    sprintf(buffer,"The temperature is: %.3f",temp);
    reply += buffer;
    reply += "<br>";
    sprintf(buffer,"The humidity is: %.3f",humid);
    reply += buffer;
    reply += "</body>";
    reply += "</html>";
    
    SERVER.send(200, "text/html", reply);
}
