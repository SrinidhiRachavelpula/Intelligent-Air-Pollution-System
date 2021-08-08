#include <ESP8266WiFi.h>
#include <MQ135.h>
#include <WiFiManager.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 8


String apiKey = "THPG7GD37ZPEZTNH"; // Enter your Write API key from ThingSpeak

const char *ssid = "AndroidAP33F5"; // replace with your wifi ssid and wpa2 key

const char *pass = "princess";

const char* server = "api.thingspeak.com";

WiFiClient client;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 60


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 


void setup()

{
  

Serial.begin(115200);

if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
{ 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
}

delay(2000);
display.display();
display.clearDisplay();



Serial.println("Connecting to ");

Serial.println(ssid);

WiFi.begin(ssid, pass);

while (WiFi.status() != WL_CONNECTED)

{

delay(500);

Serial.print(".");

}

Serial.println("");

Serial.println("WiFi connected");

pinMode(D6,INPUT);
pinMode(A0,INPUT);
int LED1 = D3;
int LED2 = D0;
int LED3 = D4;
pinMode(D3,OUTPUT);
pinMode(D0,OUTPUT);
pinMode(D4,OUTPUT);

}

void loop()

{

 float SensorValue = digitalRead(D1);
float gasSensor = analogRead(A0);

if (isnan(gasSensor))

{

Serial.println("Failed to read from MQ-5 sensor!");

return;

}



if (client.connect(server, 80)) // "184.106.153.149" or api.thingspeak.com

{

String postStr = apiKey;

postStr += "&field1=";

postStr += String(gasSensor); 

postStr += "r\n";

client.print("POST /update HTTP/1.1\n");

client.print("Host: api.thingspeak.com\n");

client.print("Connection: close\n");

client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");

client.print("Content-Type: application/x-www-form-urlencoded\n");

client.print("Content-Length: ");

client.print(postStr.length());

client.print("\n\n");

client.print(postStr);
Serial.println(gasSensor);
if (gasSensor >= 0 && gasSensor <= 100)
{
 digitalWrite(D3,HIGH);
 digitalWrite(D0,LOW);
 digitalWrite(D4,LOW);

  display.display();
 delay(1000);
}
else
if (gasSensor >= 101 && gasSensor <= 300)
{
 digitalWrite(D3,LOW);
 digitalWrite(D0,HIGH);
 digitalWrite(D4,LOW);
 
  display.display();
 delay(10000);
}
else

if (gasSensor >= 301 && gasSensor <= 800)
{
digitalWrite(D3,LOW);
 digitalWrite(D0,LOW);
 digitalWrite(D4,HIGH);

  display.display();
 delay(10000);
}
else
{
 
  
  display.display();
 }
 display.display();
display.clearDisplay();

display.setTextSize(2);
display.setTextColor(WHITE);
//display.startscrollright(0x00, 0x00);







display.setCursor(0, 0);

delay(10);
 Serial.println(gasSensor);
   
   display.print(gasSensor);
   display.println("PPM");
   if (gasSensor >= 0 && gasSensor <= 100)
   {
     Serial.println("LOW");
   
   display.println("LOW");
    
   }
   else if (gasSensor >= 101 && gasSensor <= 300)
   {
    Serial.println("MEDIUM");
   
   display.println("MEDIUM");
   }

 else  
if (gasSensor >= 301 && gasSensor <= 800)
{
   Serial.println("HIGH");
   
   display.println("HIGH");

}

else
{
   Serial.println(" VERY HIGH");
   
   display.println(" VERY HIGH");
}


Serial.println("Data Send to Thingspeak");


}


client.stop();

Serial.println("Waiting...");



// thingspeak needs minimum 15 sec delay between updates.

delay(500);

}
