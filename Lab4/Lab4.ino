#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 
#include <SimpleDHT.h>
#include <WiFi.h>
#include "time.h"

#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int pinDHT11 = 14;
SimpleDHT11 dht11(pinDHT11);

char ssid[] = "";
char pass[] = ""; 

int now=0;
int current,prev;
int h,m,s;

int set=0;

byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

const char* ntpServer = "time.stdtime.gov.tw";
const long gmtOffset_sec = 8*60*60;
int region=0;
const int daylightOffset_sec = 0;

int trig = 12;
int echo = 11;
int dua,dis;

struct tm timeinfo;

void setup() {
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  Serial.begin(115200);
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  current=millis();
  prev=millis();
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    temperature=0;
  }
  pinMode(21, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ip = ");
  Serial.println(WiFi.localIP());
  Serial.print("WiFi RSSI:");
  Serial.println(WiFi.RSSI());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  lcd.noBacklight(); 
}

void loop() {
  current=millis();
  if(current-prev>1000){
    now++;
    prev=millis();
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      temperature=0;
    }
    if(now%10==0){
      if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
      }
      // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
      char Hour[3],Min[3],Sec[3];
      strftime(Hour,3, "%H", &timeinfo);
      strftime(Min,3, "%M", &timeinfo);
      strftime(Sec,3, "%S", &timeinfo);
      // Serial.println(atoi(Hour));
      // Serial.println(atoi(Min));
      // Serial.println(atoi(Sec));
      now=atoi(Hour)*3600+atoi(Min)*60+atoi(Sec);
    }
  }
  if(digitalRead(19) == HIGH){
    region=(region+1)%2;
    if(region==0){
      configTime(8*60*60, daylightOffset_sec, ntpServer);
      now+=7*60*60;
    }else{
      configTime(60*60, daylightOffset_sec, ntpServer);
      now-=7*60*60;
    }
    while(digitalRead(19) == HIGH){
      int a=1;
    }
  }
  h=now/3600;
  m=(now%3600)/60;
  s=now%60;
  lcd.setCursor(1,0);
  if(h<10){
    lcd.print('0');
  }
  lcd.print(h);
  lcd.print(':');
  if(m<10){
    lcd.print('0');
  }
  lcd.print(m);
  lcd.print(':');
  if(s<10){
    lcd.print('0');
  }
  lcd.print(s);
  lcd.setCursor(1,1);
  if(region==0){
    lcd.print("Taiwan ");
  }else{
    lcd.print("England");
  }
  lcd.setCursor(10,1);
  lcd.print((int) temperature);
  lcd.print(".C");
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dua = pulseIn(echo,HIGH);
  dis = dua * 0.034 / 2;
  delay(50);
  if(dis<10){
    lcd.backlight(); 
  }else{
    lcd.noBacklight(); 
  }
}

