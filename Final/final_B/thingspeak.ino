void RetrieveTSChannelData() {           // Receive data from Thingspeak
  static char responseBuffer[3 * 1024];  // Buffer for received data
  client = server.available();
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  String url = "/channels/" + APIkey;                         // Start building API request string
  url += "/feeds.json?api_key=" + READAPIKEY + "&results=1";  // 5 is the results request number, so 5 are returned, 1 woudl return the last result received
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  while (!skipResponseHeaders())
    ;                           // Wait until there is some data and skip headers
  while (client.available()) {  // Now receive the data
    String line = client.readStringUntil('\n');
    if (line.indexOf('{', 0) >= 0) {  // Ignore data that is not likely to be JSON formatted, so must contain a '{'
                                      // Serial.println(line);                          // Show the text received

      Light = line.substring(line.indexOf("field1", 291) + 9, line.indexOf("field2", 291) - 3).toInt();
      Temperature = line.substring(line.indexOf("field2", 291) + 9, line.indexOf("field3", 291) - 3).toInt();
      Humidity = line.substring(line.indexOf("field3", 291) + 9, line.indexOf("field4", 291) - 3).toInt();
      Infrared = line.substring(line.indexOf("field4", 291) + 9, line.indexOf("field5", 291) - 3).toInt();
      Soil = line.substring(line.indexOf("field5", 291) + 9, line.indexOf("field6", 291) - 3).toInt();
      MQ2 = line.substring(line.indexOf("field6", 291) + 9, line.indexOf("}]}", 291) - 1).toInt();
      Serial.println(Light);
      Serial.println(Temperature);
      Serial.println(Humidity);
      Serial.println(Infrared);
      Serial.println(Soil);
      Serial.println(MQ2);
    }
  }
  client.stop();
}

bool skipResponseHeaders() {
  char endOfHeaders[] = "\r\n\r\n";  // HTTP headers end with an empty line
  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);
  if (!ok) { Serial.println("No response or invalid response!"); }
  return ok;
}
// {"channel":{"id":2396981,"name":"NYCU-Final","latitude":"0.0","longitude":"0.0","field1":"Light","field2":"Temperature","field3":"Humidity","field4":"Human Infrared","field5":"Soil","field6":"MQ2","created_at":"2024-01-08T02:44:37Z","updated_at":"2024-01-08T02:44:46Z","last_entry_id":345},"feeds":[{"created_at":"2024-01-09T03:03:08Z","entry_id":345,"field1":"1873","field2":"23","field3":"81","field4":"1","field5":"58","field6":"485"}]}
