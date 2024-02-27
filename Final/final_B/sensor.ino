void showData() {
  // humid
  int humid_r[] = { 0, 0, 0, 0, 0, 0, 0, 100, 100, 100, 100, 100, 100 };
  int humid_g[] = { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 0, 0, 0 };
  for (int i = 0; i < (13 * Humidity) / 100; i++) {
    strip1.setPixelColor(2 + i, strip1.Color(humid_r[i], humid_g[i], 0));
  }
  for (int i = (13 * Humidity) / 100; i < 13; i++) {
    strip1.setPixelColor(2 + i, strip1.Color(0, 0, 0));
  }
  // MQ2
  int MQ2_r[] = { 0, 0, 0, 100, 100, 100, 100, 100 };
  int MQ2_g[] = { 100, 100, 100, 100, 100, 100, 0, 0 };
  for (int i = 0; i < 8; i++) {
    strip1.setPixelColor(16 + i, strip1.Color(MQ2_r[min(7, (MQ2 - 300) / 100)], MQ2_g[min(7, (MQ2 - 300) / 100)], 0));
  }
  // Soil
  int Soil_r[] = { 0, 0, 0, 100, 100, 100, 100, 100 };
  int Soil_g[] = { 100, 100, 100, 100, 100, 100, 0, 0 };
  for (int i = 0; i < min(8, (MQ2 - 300) / 100); i++) {
    strip1.setPixelColor(24 + i, strip1.Color(MQ2_r[i], MQ2_g[i], 0));
  }
  for (int i = min(8, (MQ2 - 300) / 100); i < 8; i++) {
    strip1.setPixelColor(24 + i, strip1.Color(0, 0, 0));
  }
  strip1.show();
  strip2.setPixelColor(0, strip2.Color(0, 0, 0));
  // Light;
  if (Light < 1000) {
    strip2.setPixelColor(1, strip2.Color(100, 100, 100));
  } else {
    strip2.setPixelColor(1, strip2.Color(0, 0, 0));
  }
  strip2.setPixelColor(2, strip2.Color(0, 0, 0));
  // Temperature;
  if (Temperature > 28) {
    strip2.setPixelColor(3, strip2.Color(0, 0, 100));
  } else if (Temperature < 10) {
    strip2.setPixelColor(3, strip2.Color(100, 0, 0));
  } else {
    strip2.setPixelColor(3, strip2.Color(0, 100, 0));
  }
  strip2.setPixelColor(4, strip2.Color(0, 0, 0));
  // Infrared;
  if (Infrared == 1) {
    strip2.setPixelColor(5, strip2.Color(100, 100, 100));
  } else {
    strip2.setPixelColor(5, strip2.Color(0, 0, 0));
  }
  strip2.setPixelColor(6, strip2.Color(0, 0, 0));
  if (MQ2 > 600) {
    strip2.setPixelColor(7, strip2.Color(100, 100, 100));
  } else {
    strip2.setPixelColor(7, strip2.Color(0, 0, 0));
  }
  strip2.show();
}
// 0000000000000000
// Temp:11
// Humid:11
// Light:11
// Infrared:11
// Soil:11
// MQ2:11
void lcdShowData() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(Temperature) + " C");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("Humid: " + String(Humidity) + "%");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Light: " + String(int(Light * 100 / 4096)) + "%");
  lcd.setCursor(0, 1);
  if (Infrared == 1) {
    lcd.print("Infrared: True  ");
  } else {
    lcd.print("Infrared: False ");
  }
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Soil: " + String(int(Soil * 100 / 1024)) + "%");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("MQ2: " + String(MQ2) + "ppm");
  delay(2000);
}
