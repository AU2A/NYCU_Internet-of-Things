#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>

#define WAKEUP_MS 30 * 1000000

char ssid[] = "";
char password[] = "";
String url = "http://api.thingspeak.com/update?api_key=";

String data="";

int pinDHT11 = 14;
SimpleDHT11 dht11(pinDHT11);
byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

void setup() {
  Serial.begin(115200);
  Serial.printf("喚醒");

  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("connect...");
  pinMode(LED_BUILTIN,OUTPUT);
  
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    temperature=0;
  }

  data=url + "&field1=" + (int) temperature;

  Serial.println((int) temperature);
  if(temperature<20||30<temperature){
    digitalWrite(LED_BUILTIN,HIGH);
  }else{
    digitalWrite(LED_BUILTIN,LOW);
  }
  
  HTTPClient http;
  http.begin(data);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK){
    String payload = http.getString();
    // Serial.print("網頁內容=");
    // Serial.println(payload);
    Serial.println("網路傳送成功");
  } else {
    Serial.println("網路傳送失敗");
  }
  http.end();

  delay(10000);
  Serial.printf("睡眠");
  esp_sleep_enable_timer_wakeup(WAKEUP_MS);
  esp_deep_sleep_start();
}

void loop(){

}