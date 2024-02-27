#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 

#define I2C_SDA 17
#define I2C_SCL 18

LiquidCrystal_I2C lcd(0x27, 16, 2); 

byte I_rev[8]= {B10001,B11011,B11011,B11011,B11011,B11011,B10001};
byte heart[8]= {B01010,B11111,B11111,B01110,B00100,B00000,B00000};
byte heart_rev[8]= {B01010,B10001,B10001,B01010,B00100,B00000,B00000};
byte N_rev[8]= {B01110,B01110,B00110,B01010,B01100,B01110,B01110};
byte Y_rev[8]= {B01110,B01110,B01110,B10101,B11011,B11011,B11011};
byte C_rev[8]= {B10001,B01110,B01111,B01111,B01111,B01110,B10001};
byte U_rev[8]= {B01110,B01110,B01110,B01110,B01110,B01110,B10001};
byte S_rev[8]= {B10000,B01111,B01111,B10001,B11110,B11110,B00001};


void setup() {
  Wire.begin(I2C_SDA, I2C_SCL);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, I_rev);
  lcd.createChar(1, heart);
  lcd.createChar(2, heart_rev);
  lcd.createChar(3, N_rev);
  lcd.createChar(4, Y_rev);
  lcd.createChar(5, C_rev);
  lcd.createChar(6, U_rev);
  lcd.createChar(7, S_rev);

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("I ");
  lcd.write(1);
  lcd.print(" NYCU CS");
  delay(1000);
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.setCursor(2,0);
  lcd.write(2);
  lcd.setCursor(4,0);
  lcd.write(3);
  lcd.write(4);
  lcd.write(5);
  lcd.write(6);
  lcd.setCursor(9,0);
  lcd.write(5);
  lcd.write(7);
  delay(1000);

}














