#define BLYNK_TEMPLATE_ID "TMPL3-KbSGWp0"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define DPIN 2      //Pin to connect DHT sensor (GPIO number) D2
#define DTYPE DHT11  // Define DHT 11 or DHT22 sensor type

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SSD1306_I2C_ADDRESS 0x3D



#define BLYNK_AUTH_TOKEN "1mVfo9WkV2cgwPKXFmd8UZLYn65zxMD_"
#define BLYNK_PRINT Serial



char ssid[] = "Galaxy M214030";
char pass[] = "pnnp9984";
BlynkTimer timer;


///Sensor Values
float tc;
float hu;
float mq_value;
int relay1 = 0;
int relay2 = 0;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

DHT dht(DPIN, DTYPE);


int buzzerPin = D5;

int relay1Pin = D6;
int relay2Pin = D7;


bool alarmMode = false;
float tempLimit = 60;
float gasLevelLimit = 150;



BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(relay1Pin,HIGH);  // Set digital pin 2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(relay1Pin,LOW);  // Set digital pin 2 LOW    
  }
}

BLYNK_WRITE(V1)
{
  // Set incoming value from pin V0 to a variable
  if(param.asInt() == 1)
  {
    // execute this code if the switch widget is now ON
    digitalWrite(relay2Pin,HIGH);  // Set digital pin 2 HIGH
  }
  else
  {
    // execute this code if the switch widget is now OFF
    digitalWrite(relay2Pin,LOW);  // Set digital pin 2 LOW    
  }
}

BLYNK_CONNECTED()
{

}


void myTimerEvent() {

  Blynk.virtualWrite(V5, tc);
  Blynk.virtualWrite(V6, hu);
  Blynk.virtualWrite(V2, mq_value);

}


void setup() {

  pinMode(5 , INPUT);

  pinMode(buzzerPin, OUTPUT); 

  Serial.begin(9600);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC , 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(F("Hello, World!"));
  display.display();


  ////////////////////////
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);


  timer.setInterval(1000L, myTimerEvent); 

}

void loop() {
  delay(1000);

  int pirValue = digitalRead(0);
  Serial.print("PIR : ");
  Serial.print(pirValue);


  mq_value = analogRead(A0);
  Serial.print("MQ Value: ");
  Serial.print(mq_value);

  tc = dht.readTemperature(false);  //Read temperature in C
  hu = dht.readHumidity();          //Read Humidity

  Serial.print("Temp: ");
  Serial.print(tc);
  Serial.print(" C, ");
  Serial.print(" F, Hum: ");
  Serial.print(hu);
  Serial.println("%");

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("Gas Level:");
  display.setCursor(0, 20);
  display.print("Value: ");
  display.print(mq_value, 2);


  display.setCursor(0, 40);
  display.println("DHT: ");
  display.print(tc, 2);
  display.print(" C");
  display.print(" ");
  display.print(hu, 2);
  display.print(" %");

  display.display();

  if(tc > tempLimit || mq_value > gasLevelLimit || hu > 68) {
    alarmMode = true;
  } else {
    alarmMode = false;
  }


  if(alarmMode) {
    tone(buzzerPin, 1000, 250); // Parameters: (pin, frequency in Hz, duration in milliseconds)
    delay(500);
  }
  

  Blynk.run();
  timer.run();

}