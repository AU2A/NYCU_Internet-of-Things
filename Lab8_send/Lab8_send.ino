#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>
char ssid[] = "-";
char password[] = "-";
String url = "http://api.thingspeak.com/update?api_key=-";

int trig = 12;
int echo = 11;
int dua,dis=100,current,prev;
String data="";

void setup() {
  Serial.begin(115200);
  Serial.print("...");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");
  pinMode(LED_BUILTIN,OUTPUT);
    
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);

  current=millis();
  prev=millis();
}

void loop() {

  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dua = pulseIn(echo,HIGH);
  dis = dua * 0.034 / 2;
  delay(100);
  current=millis();
  if(current-prev>20000){
    prev=millis();
    if(dis<=10){
      Serial.println("HIGH");
      data=url + "&field1=1";
    }else{
      Serial.println("LOW");
      data=url + "&field1=0";
    }
    HTTPClient http;
    http.begin(data);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK){
      String payload = http.getString();
      // Serial.print("網頁內容=");
      // Serial.println(payload);
    } else {
      Serial.println("網路傳送失敗");
    }
    http.end();
  }
  if(dis<=10){
    if(current%1000>500){
      digitalWrite(LED_BUILTIN,HIGH);
    }else{
      digitalWrite(LED_BUILTIN,LOW);
    }
  }else{
    digitalWrite(LED_BUILTIN,LOW);
  }
}