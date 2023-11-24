#include <WiFi.h>
#include <HTTPClient.h>
#include <SimpleDHT.h>
char ssid[] = "-";
char password[] = "-";
String url = "http://api.thingspeak.com/update?api_key=-";
int pinDHT11 = 17;
SimpleDHT11 dht11(pinDHT11);
int pPin = 14;

byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

int current,prev;
int tempold=0,humidold=0,photoresistorold=0;
int temp=0,humid=0,photoresistor=0;
int cnt=0,tcnt=0,hcnt=0,pcnt=0;
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
  pinMode(pPin, INPUT);  

  current=millis();
  prev=millis();
}

void loop() {
  current=millis();
  if(current-prev>5000){
    prev=millis();
    cnt=(cnt+1)%2;
    //Read Data
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      return;
    }
    temp=(int)temperature;
    humid=(int)humidity;
    photoresistor=analogRead(pPin);
    data=url;
    if(temp!=tempold){
      data=data+"&field1=" + temp;
      tempold=temp;
      tcnt=0;
    }else{
      tcnt++;
      if(tcnt>=9){
      data=data+"&field1=" + temp;
        tcnt=0;
      }
    }
    if(humid!=humidold){
      data=data+"&field2=" + humid;
      humidold=humid;
      hcnt=0;
    }else{
      hcnt++;
      if(hcnt>=9){
        data=data+"&field2=" + humid;
        hcnt=0;
      }
    }
    if(cnt%2==0){
      if(photoresistor!=photoresistorold){
        data=data+"&field3=" + photoresistor;
        photoresistorold=photoresistor;
        pcnt=0;
      }else{
        pcnt++;
        if(pcnt>=9){
          data=data+"&field3=" + photoresistor;
          pcnt=0;
        }
      }
    }
    if(data!=url){
      HTTPClient http;
      String url1 = data + "&field4=2";
      Serial.println(url1);

      http.begin(url1);
      int httpCode = http.GET();
      if (httpCode == HTTP_CODE_OK){
        String payload = http.getString();
        Serial.print("網頁內容=");
        Serial.println(payload);
      } else {
        Serial.println("網路傳送失敗");
      }
      http.end();
    }
  }
}