#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char *ssid =  "FAST_0456";     // Enter your WiFi Name
const char *pass =  "lw730208"; // Enter your WiFi Password
const int AirValue = 600; //you need to change this value that you had recorded in t
const int WaterValue = 400; //you need to change this value that you had recorded in th
int intervals = (AirValue - WaterValue)/3;

uint8_t LedR = A4;  // GPIO32
uint8_t LedG = A5;  // GPIO33
uint8_t LedB = A18; // GPIO25

WiFiClient client;
#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "gada888" // Your Adafruit IO Username
#define MQTT_PASS "aio_FKFS66ZOi9180nZrq2q7up6Ydv6A" // Adafruit IO AIO key

const int moisturePin = A0;             // moisteure sensor pin
int mValue;              //moisture reading

//Set up the feed you're publishing to
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
// mData is the feed name where you will publish your data
Adafruit_MQTT_Publish mData = Adafruit_MQTT_Publish(&mqtt,MQTT_NAME "/f/mData");  


void setup()
{
  Serial.begin(115200);
  delay(10);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");              // print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
}
 void loop()
{
  MQTT_connect();
  mValue = (355 - ((analogRead(moisturePin) / 1023.00) * 100));
  Serial.print("Soil Moisture is  = ");
  Serial.print(mValue);
  Serial.println("%");

if(mValue > 90)
{
Serial.println("Very Wet");
    digitalWrite(LedR, LOW);
    digitalWrite(LedB, LOW);
    digitalWrite(LedG, HIGH); //Green LED
}
else if((mValue < 20))
{
Serial.println("Dry");
    digitalWrite(LedR, HIGH);
    digitalWrite(LedB, LOW);
    digitalWrite(LedG, LOW); //Green LED
} 

  String data1 = String(mValue);
//This condition is used to publish the Variable (mValue) on adafruit IO. Change thevariable according to yours.
       if (! mData.publish(mValue)) 
       {                     
         delay(2000);   
          }
 delay(3000);
}

void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
       mqtt.disconnect();
       delay(3000);  // wait 5 seconds
       retries--;
       if (retries == 0) 
       {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
}