#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 
#include <SimpleDHT.h>

#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); 

int pinDHT11 = 14;
SimpleDHT11 dht11(pinDHT11);

int now=0;
int current,prev;
int h,m,s;

int set=0;

byte temperature = 0;
byte humidity = 0;
int err = SimpleDHTErrSuccess;

void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  current=millis();
  prev=millis();
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    temperature=0;
  }
  pinMode(21, INPUT);
  pinMode(20, INPUT);
  pinMode(19, INPUT);
}

void loop() {
  current=millis();
  if(current-prev>1000){
    now++;
    prev=millis();
    if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      temperature=0;
    }
  }
  if(digitalRead(21) == HIGH){
    set=(set+1)%4;
    while(digitalRead(21) == HIGH){
      int a=1;
    }
  }
  if(digitalRead(20) == HIGH){
    if(set==1){
        now+=3600;
    }else if(set==2){
        now+=60;
    }else if(set==3){
        now+=1;
    }
    while(digitalRead(20) == HIGH){
      int a=1;
    }
  }
  if(digitalRead(19) == HIGH){
    if(set==1&&now>=3600){
        now-=3600;
    }else if(set==2&&now>=60){
        now-=60;
    }else if(set==3&&now>=1){
        now-=1;
    }
    while(digitalRead(19) == HIGH){
      int a=1;
    }
  }
  h=now/3600;
  m=(now%3600)/60;
  s=now%60;
  lcd.setCursor(1,0);
  if(h<10){
    lcd.print('0');
  }
  lcd.print(h);
  lcd.print(':');
  if(m<10){
    lcd.print('0');
  }
  lcd.print(m);
  lcd.print(':');
  if(s<10){
    lcd.print('0');
  }
  lcd.print(s);
  lcd.setCursor(1,1);
  if(set==1){
    lcd.print("^^      ");
  }else if(set==2){
    lcd.print("   ^^   ");
  }else if(set==3){
    lcd.print("      ^^");
  }else{
    lcd.print("        ");
  }
  lcd.setCursor(10,1);
  lcd.print((int) temperature);
  lcd.print(".C");
}

