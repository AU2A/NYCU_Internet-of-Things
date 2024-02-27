void getSensorData() {
  getLight();
  getTempHumid();
  getHumanInfrared();
  getSoilHumid();
  getMQ2();
}

void printSensorData() {
  Serial.println("--------------------");
  Serial.print("Light:       ");
  Serial.println(Light);
  Serial.print("Temperature: ");
  Serial.println(Temperature);
  Serial.print("Humidity:    ");
  Serial.println(Humidity);
  Serial.print("Infrared:    ");
  Serial.println(Infrared);
  Serial.print("Soil:        ");
  Serial.println(Soil);
  Serial.print("MQ2:         ");
  Serial.println(MQ2);
  Serial.println("--------------------");
}

int getLight() {
  Light = analogRead(pinLight);
  return Light;
}

int getTempHumid() {
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(pinTempHumid, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    return -1;
  }
  Temperature = (int)temperature;
  Humidity = (int)humidity;
  return Temperature;
}

int getHumanInfrared() {
  Infrared = digitalRead(pinInfrared);
  return Infrared;
}

int getSoilHumid() {
  Soil = analogRead(pinSoil);
  return Soil;
}

int getMQ2() {
  MQ2 = analogRead(pinMQ2);
  return MQ2;
}