int trig = 12;
int echo = 11;
int dua,dis,current,prev;
bool light=false;


void setup() {
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  prev=millis();
}

void loop() {
  current=millis();
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  dua = pulseIn(echo,HIGH);
  dis = dua * 0.034 / 2;
  delay(50);
  if(dis<10){
    if(current-prev>100){
      if(light){
        digitalWrite(LED_BUILTIN,LOW);
        light=false;
      }else{
        digitalWrite(LED_BUILTIN,HIGH);
        light=true;
      }
      prev=millis();
    }
  }else if(dis<20){
    if(current-prev>500){
      if(light){
        digitalWrite(LED_BUILTIN,LOW);
        light=false;
      }else{
        digitalWrite(LED_BUILTIN,HIGH);
        light=true;
      }
      prev=millis();
    }
  }else{
    if(light){
      digitalWrite(LED_BUILTIN,LOW);
      light=false;
    }
  }
  Serial.println(dis);
}


