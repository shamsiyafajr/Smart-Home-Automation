//Linking the Bynk to Wokwi 
#define BLYNK_TEMPLATE_ID "TMPL5-ezJbzLt"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "stjXYnSzl8MF9mlRmEIi-d6HMUhuwBej"

// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define I2C_ADDR 0x27
#define LCD_COLUMNS 16
#define LCD_LINES 2

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"

// Variable for motion sensing
#define PIR_SENSOR  4
//int flag=0;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

char auth[] = BLYNK_AUTH_TOKEN;

//  WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

BlynkTimer timer;

// Define pin assignments

#define button1_pin 26
#define button2_pin 25
#define button3_pin 33
#define button4_pin 32

#define relay1_pin 13
#define relay2_pin 12
#define relay3_pin 14
#define relay4_pin 27

int relay1_state = 0;
int relay2_state = 0;
int relay3_state = 0;
int relay4_state = 0;

// Change the virtual pins according to the rooms
#define button1_vpin V1
#define button2_vpin V2
#define button3_vpin V3
#define button4_vpin V4

// DHT22
#define DHTPIN 2     // Pin where the DHT11 is connected
#define DHTTYPE DHT22   // DHT 11 sensor type

DHT dht(2, DHT22);

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  Blynk.syncVirtual(button1_vpin);
  Blynk.syncVirtual(button2_vpin);
  Blynk.syncVirtual(button3_vpin);
  Blynk.syncVirtual(button4_vpin);
}

// This function is called every time the Virtual Pin state changes
// i.e., when the web push switch from Blynk App or Web Dashboard
BLYNK_WRITE(button1_vpin) {
  relay1_state = param.asInt();
  digitalWrite(relay1_pin, relay1_state);
}

BLYNK_WRITE(button2_vpin) {
  relay2_state = param.asInt();
  digitalWrite(relay2_pin, relay2_state);
}

BLYNK_WRITE(button3_vpin) {
  relay3_state = param.asInt();
  digitalWrite(relay3_pin, relay3_state);
}

BLYNK_WRITE(button4_vpin) {
  relay4_state = param.asInt();
  digitalWrite(relay4_pin, relay4_state);
}

void setup() {
  pinMode(PIR_SENSOR, INPUT);

  // Debug console

  pinMode(button1_pin, INPUT_PULLUP);
  pinMode(button2_pin, INPUT_PULLUP);
  pinMode(button3_pin, INPUT_PULLUP);
  pinMode(button4_pin, INPUT_PULLUP);

  pinMode(relay1_pin, OUTPUT);
  pinMode(relay2_pin, OUTPUT);
  pinMode(relay3_pin, OUTPUT);
  pinMode(relay4_pin, OUTPUT);

  // During startup, all Relays should be turned OFF
  digitalWrite(relay1_pin, HIGH);
  digitalWrite(relay2_pin, HIGH);
  digitalWrite(relay3_pin, HIGH);
  digitalWrite(relay4_pin, HIGH);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  dht.begin(); //dht starts 
  lcd.init();
  lcd.backlight();
  lcd.begin(20, 4);
  lcd.setCursor(3, 0);
  lcd.print("Home Automation");
  lcd.setCursor(5, 2);
  lcd.print("By Shamsiya");

  timer.setInterval(5000L, notifyOnTheft);//to delay the function call after the first call I have given the delay in blynk event creation 
}

void loop() {
  Blynk.run();
  timer.run();

  // Read temperature and humidity values from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if reading is successful
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  //Serial.print("Temperature: ");
  //Serial.print(temperature);
  //Serial.print(" °C, Humidity: ");
  //Serial.print(humidity);
  //Serial.println(" %"); these code makes the program slower but helps to check the error

  
  // Display temperature and humidity on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP:");
  lcd.setCursor(8, 0);
  lcd.print(temperature);
  lcd.setCursor(0, 2);
  lcd.print("HUMID:");
  lcd.setCursor(8, 2);
  lcd.print(humidity);
  Blynk.virtualWrite(V5, humidity);     // Send humidity value to Blynk App
    Blynk.virtualWrite(V6, temperature);  // Send temperature value to Blynk App

  // Listen for button presses
  listenPushButtons();

}

// Function to handle button presses
void listenPushButtons() {
  if (digitalRead(button1_pin) == LOW) {
    //delay(50);
    controlRelay(1);
    Blynk.virtualWrite(button1_vpin, relay1_state); // Update button state
  }
  else if (digitalRead(button2_pin) == LOW) {
    //delay(50);
    controlRelay(2);
    Blynk.virtualWrite(button2_vpin, relay2_state); // Update button state
  }
  else if (digitalRead(button3_pin) == LOW) {
    //delay(50);
    controlRelay(3);
    Blynk.virtualWrite(button3_vpin, relay3_state); // Update button state
  }
  else if (digitalRead(button4_pin) == LOW) {
    //delay(50);
    controlRelay(4);
    Blynk.virtualWrite(button4_vpin, relay4_state); // Update button state
  }
}

// Function to control relays based on button presses
void controlRelay(int relay) {
  if (relay == 1) {
    relay1_state = !relay1_state;
    digitalWrite(relay1_pin, relay1_state);
    Serial.print("Relay1 State = ");
    Serial.println(relay1_state);
    //delay(50);
  }
  else if (relay == 2) {
    relay2_state = !relay2_state;
    digitalWrite(relay2_pin, relay2_state);
    Serial.print("Relay2 State = ");
    Serial.println(relay2_state);
    //delay(50);
  }
  else if (relay == 3) {
    relay3_state = !relay3_state;
    digitalWrite(relay3_pin, relay3_state);
    Serial.print("Relay3 State = ");
    Serial.println(relay3_state);
   // delay(50);
  }
  else if (relay == 4) {
    relay4_state = !relay4_state;
    digitalWrite(relay4_pin, relay4_state);
    Serial.print("Relay4 State = ");
    Serial.println(relay4_state);
    //delay(50);
  }
}

// Function to handle theft alerts
void notifyOnTheft()
{
  int isTheftAlert = digitalRead(PIR_SENSOR);
  Serial.println(isTheftAlert);
  if (isTheftAlert==1) {
    Serial.println("Theft Alert in Home");

    // I tried to use manual push notification but creating it in Blynk is easier
    //Blynk.email("shamsiyashamz@gmail.com", "Alert", "Theft Alert in Home");
    //Blynk.notify("Alert : Theft Alert in Home");
    Blynk.logEvent("theft_alert","Theft Alert in Home");
    //flag=1;
  }
  else if (isTheftAlert==0)
  {
   //flag=0;
  }
}
//tried using Automation feature in blynk and connected my phone similar to events but configuration takes a bit longer
/*void sendSensor() {
  int motion_data = digitalRead(motion_sensor);
  Serial.println(motion_data);
  Blynk.virtualWrite(V7, motion_data);
}*/
