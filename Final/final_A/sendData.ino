
void sendDataToThingspeak() {
  HTTPClient http;
  String sendURL = thingspeakURL;
  sendURL += "&field1=" + String(Light);
  sendURL += "&field2=" + String(Temperature);
  sendURL += "&field3=" + String(Humidity);
  sendURL += "&field4=" + String(Infrared);
  sendURL += "&field5=" + String(Soil);
  sendURL += "&field6=" + String(MQ2);
  // Serial.println(sendURL);
  http.begin(sendURL);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.print("網頁內容=");
    Serial.println(payload);
  } else {
    Serial.println("網路傳送失敗");
  }
  http.end();
}

void sendDataToLine() {
  String message = "\n檢測環境發生異常：\n";
  int LEN = message.length();
  if (Temperature > 30) {
    message += "溫度太高\n";
  } else if (Temperature < 10) {
    message += "溫度太低\n";
  }
  if (Humidity > 85) {
    message += "濕度太高\n";
  } else if (Humidity < 30) {
    message += "濕度太低\n";
  }
  if (MQ2 > 1000) {
    message += "環境品質不佳\n";
  }
  else if (MQ2 > 1500) {
    message += "環境品質嚴重不佳\n";
  }
  if (message.length() > LEN) {
    Serial.println(message);
    if (clientSecure.connect(lineHost, 443)) {
      LEN = message.length();
      String url = "/api/notify";
      clientSecure.println("POST " + url + " HTTP/1.1");
      clientSecure.print("Host: ");
      clientSecure.println(lineHost);
      clientSecure.print("Authorization: Bearer ");
      clientSecure.println(Linetoken);
      clientSecure.println("Content-Type: application/x-www-form-urlencoded");
      clientSecure.print("Content-Length: ");
      clientSecure.println(String((LEN + 8)));
      clientSecure.println();
      clientSecure.print("message=");
      clientSecure.println(message);
      clientSecure.println();
      // delay(2000);
      // String response = clientSecure.readString();
      // Serial.println(response);
      clientSecure.stop();
    } else {
      //傳送失敗
      Serial.println("connected fail");
    }
  }
}