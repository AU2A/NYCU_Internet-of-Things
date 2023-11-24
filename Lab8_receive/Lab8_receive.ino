#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
WiFiServer server(80); 

char   host[] = "api.thingspeak.com"; // ThingSpeak address
const int httpPort = 80;

String Linetoken = "-";
WiFiClientSecure Sclient;//網路連線物件
char linehost[] = "notify-api.line.me";//LINE Notify API網址

// You Need to Modify
String CHANNELID = "1";  // Thingspeak CHANNEL ID
String APIkey="-";
String READAPIKEY = "-";  // READ APIKEY for the CHANNEL_ID
 
// You Need to Modify 
const char *ssid     = "-"; 
const char *password = "-"; 

WiFiClient client;  
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server

bool notice=false;
int current,prev;

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Sclient.setInsecure();//ESP32核心 1.0.6以上
  pinMode(LED_BUILTIN,OUTPUT);
  current=millis();
  prev=millis();
}

void loop(){
  RetrieveTSChannelData();
  delay(60000);
}

void RetrieveTSChannelData() {  // Receive data from Thingspeak
  static char responseBuffer[3*1024]; // Buffer for received data
  client = server.available(); 
  if (!client.connect(host, httpPort)) { 
    Serial.println("connection failed"); 
    return; 
  } 
  String url = "/channels/" + APIkey; // Start building API request string
  url += "/fields/1.json?api_key=" + READAPIKEY + "&results=1";  // 5 is the results request number, so 5 are returned, 1 woudl return the last result received
  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  while (!skipResponseHeaders());                      // Wait until there is some data and skip headers
  while (client.available()) {                         // Now receive the data
    String line = client.readStringUntil('\n');
    if ( line.indexOf('{',0) >= 0 ) {                  // Ignore data that is not likely to be JSON formatted, so must contain a '{'
      Serial.println(line);                            // Show the text received
      line.toCharArray(responseBuffer, line.length()); // Convert to char array for the JSON decoder
      decodeJSON(responseBuffer);                      // Decode the JSON text
    }
  }
  client.stop();
}

bool skipResponseHeaders() { 
  char endOfHeaders[] = "\r\n\r\n"; // HTTP headers end with an empty line 
  client.setTimeout(HTTP_TIMEOUT); 
  bool ok = client.find(endOfHeaders); 
  if (!ok) { Serial.println("No response or invalid response!"); } 
  return ok; 
} 

bool decodeJSON(char *json) {
  StaticJsonBuffer <3*1024> jsonBuffer;
  char *jsonstart = strchr(json, '{'); // Skip characters until first '{' found and ignore length, if present
  if (jsonstart == NULL) {
    Serial.println("JSON data missing");
    return false;
  }
  json = jsonstart;
  JsonObject& root = jsonBuffer.parseObject(json); // Parse JSON
  if (!root.success()) {
    Serial.println(F("jsonBuffer.parseObject() failed"));
    return false;
  }
  JsonObject& root_data = root["channel"]; // Begins and ends within first set of { }
  String id   = root_data["id"];
  String name = root_data["name"];
  String field1_name = root_data["field1"];
  String datetime    = root_data["updated_at"];
  Serial.println("\n\n Channel id: "+id+" Name: "+ name);
  Serial.println(" Readings last updated at: "+datetime);
  
  for (int result = 0; result < 1; result++){
    JsonObject& channel = root["feeds"][result]; // Now we can read 'feeds' values and so-on
    String entry_id     = channel["entry_id"];
    String field1value  = channel["field1"];
    Serial.print(" Field1 entry number ["+entry_id+"] had a value of: ");
    Serial.println(field1value);
    if(field1value=="1"){
      sendLine();
    }else{
      notice=false;
    }
  }
}

void sendLine(){
  String message = "偵測有人！";
  Serial.println(message);
  if (Sclient.connect(linehost, 443)) {
    int LEN = message.length();
    //傳遞POST表頭
    String url = "/api/notify";
    Sclient.println("POST " + url + " HTTP/1.1");
    Sclient.print("Host: "); Sclient.println(linehost);
    //權杖
    Sclient.print("Authorization: Bearer "); 
    Sclient.println(Linetoken);
    Sclient.println("Content-Type: application/x-www-form-urlencoded");
    Sclient.print("Content-Length: "); 
    Sclient.println( String((LEN + 8)) );
    Sclient.println();      
    Sclient.print("message="); 
    Sclient.println(message);
    Sclient.println();
    //等候回應
    delay(2000);
    String response = Sclient.readString();
    //顯示傳遞結果
    Serial.println(response);
    Sclient.stop(); //斷線，否則只能傳5次
  }
  else {
    //傳送失敗
    Serial.println("connected fail");
  }
}