int ledPin = 20;       
int inputPin = 14;      

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);       
}

void loop() {
  Serial.println(analogRead(inputPin));
  analogWrite(ledPin,255-analogRead(inputPin)*255/4096);
}

