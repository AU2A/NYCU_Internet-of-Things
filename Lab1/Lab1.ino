// 312552026
// 蔡濟謙


int RED = 10;
int GREEN = 12;
int YELLOW = 11;
int nRED = 17;
int nGREEN = 15;
int nYELLOW = 16;
bool on=HIGH;

void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(YELLOW,OUTPUT);
  pinMode(nRED,OUTPUT);
  pinMode(nGREEN,OUTPUT);
  pinMode(nYELLOW,OUTPUT);
}

void loop() {
  digitalWrite(GREEN,on);
  delay(5000);
  digitalWrite(GREEN,!on);
  for(int i=0;i<10;i++){
    digitalWrite(YELLOW,on);
    delay(100);
    digitalWrite(YELLOW,!on);
    delay(100);
  }
  digitalWrite(RED,on);
  delay(5000);
  digitalWrite(RED,!on);
  
  digitalWrite(nGREEN,!on);
  delay(5000);
  digitalWrite(nGREEN,on);
  for(int i=0;i<10;i++){
    digitalWrite(nYELLOW,!on);
    delay(100);
    digitalWrite(nYELLOW,on);
    delay(100);
  }

  digitalWrite(nRED,!on);
  delay(5000);
  digitalWrite(nRED,on);
}
