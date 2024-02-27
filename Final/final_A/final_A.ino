#define CAMERA_MODEL_ESP32S3_EYE
#include "Arduino.h"
#include "camera_pins.h"
#include "driver/rtc_io.h"
#include "esp_camera.h"
#include "soc/soc.h"           //用於電源不穩不重開機
#include "soc/rtc_cntl_reg.h"  //用於電源不穩不重開機
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <SimpleDHT.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

#define I2C_SDA 1
#define I2C_SCL 2

const char* ssid = "";
const char* password = "";

// URL
String thingspeakURL = "http://api.thingspeak.com/update?api_key=";
char post_url[] = "";
char lineHost[] = "notify-api.line.me";
bool internet_connected = false;

String Linetoken = "";

// Thingspeak
char host[] = "api.thingspeak.com";  // ThingSpeak address
const int httpPort = 80;
WiFiServer server(80);
String APIkey = "";
String READAPIKEY = "";  // READ APIKEY for the CHANNEL_ID
WiFiClient client;
WiFiClientSecure clientSecure;
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server

// Pin Define
int pinLight = 19;
int pinTempHumid = 20;
int pinInfrared = 21;
int pinSoil = 1;
int pinMQ2 = 2;

// Sensor Define
SimpleDHT11 dht11(pinTempHumid);

// Sensor Data
int Light;
int Temperature;
int Humidity;
int Infrared;
int Soil;
int MQ2;

// time val
int current, prev, count = 0;

void startCameraServer();
// void setupLedFlash(int pin);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Use http://");
  Serial.println(WiFi.localIP());
  clientSecure.setInsecure();
  cameraInit();

  // pinMode
  pinMode(pinLight, INPUT);
  pinMode(pinInfrared, INPUT);
  pinMode(pinMQ2, INPUT);
  pinMode(pinSoil, INPUT);

  //time Init
  current = millis();
  prev = millis();
}

void loop() {
  // current = millis();
  // if (current - prev > 2000) {
  //   prev = millis();
  //   count = (count + 1) % 10;
  //   if (count % 10 == 0) {
  //   }
  //   if (count % 20 == 0) {
  //   }
  // }
  getSensorData();
  printSensorData();
  sendDataToLine();
  sendDataToThingspeak();
  delay(20000);
}
