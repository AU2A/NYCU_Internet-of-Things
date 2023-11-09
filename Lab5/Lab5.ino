#include <WiFi.h>
#include <SimpleDHT.h>

const char* ssid = "";
const char* password = "";

WiFiServer server(80);

String header;

IPAddress local_IP(192, 168, 1, 1); 
 
IPAddress gateway(192, 168, 1, 10); 

IPAddress subnet(255, 255, 255, 0); 
//IPAddress primaryDNS(8, 8, 8, 8); //optional 
//PAddress secondaryDNS(8, 8, 4, 4); //optional

int pinDHT11 = 19;
SimpleDHT11 dht11(pinDHT11);
byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;
bool unit=false;

int current,prev;

void setup() {
  Serial.begin(115200);

  Serial.print("Setting AP (Access Point)…");
  WiFi.softAPConfig(local_IP, local_IP, subnet);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  server.begin();

  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    temperature=0;
  }

  current=millis();
  prev=millis();
}



void loop(){
  WiFiClient client = server.available();
  if (client) {                             // If a new client connects,
      current=millis();
      if(current-prev>1000){
        prev=millis();
        do{
          if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
            temperature=0;
          }
        }while(temperature==0);
        Serial.println((int)temperature);
      }
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /unit/on") >= 0) {
              unit = true;
            } 
            else if (header.indexOf("GET /unit/off") >= 0) {
              unit = false;
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            client.println("<body><h1>ESP32 Temp Server</h1>");
            
            
            if (unit==false) {
              client.print("<p>Temp ");
              client.print((int)temperature);
              client.println(" .C</p>");
              client.println("<p><a href=\"/unit/on\"><button class=\"button\">Fahrenheit</button></a></p>");
            } else {
              client.print("<p>Temp ");
              client.print((int)temperature*9/5+32);
              client.println(" .F</p>");
              client.println("<p><a href=\"/unit/off\"><button class=\"button\">Celsius</button></a></p>");
            } 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } 
          else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } 
        else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
