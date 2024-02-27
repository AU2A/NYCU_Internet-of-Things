#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"
#include "esp_camera.h"
#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>

#define I2C_SDA 1
#define I2C_SCL 2

const char* ssid = "";
const char* password = "";


// Thingspeak
char host[] = "api.thingspeak.com";  // ThingSpeak address
const int httpPort = 80;
WiFiServer server(80);
String APIkey = "";
String READAPIKEY = "";  // READ APIKEY for the CHANNEL_ID
WiFiClient client;
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server

// Sensor Data
int Light;
int Temperature;
int Humidity;
int Infrared;
int Soil;
int MQ2;

Adafruit_NeoPixel strip1(32, 19, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(8, 20, NEO_GRB + NEO_KHZ800);

LiquidCrystal_I2C lcd(0x27, 16, 2);

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
  cameraInit();
  strip1.begin();
  strip2.begin();
  strip1.setBrightness(20);
  strip2.setBrightness(20);
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
}

void loop() {
  RetrieveTSChannelData();
  showData();
  lcdShowData();
  // delay(20000);
}
