#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const int interval = 10; // Read temperature every n seconds
float timer = interval * 1000;

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  if (timer >= interval) {
    OneWire oneWire(0);
    DallasTemperature sensors(&oneWire);
    sensors.begin();  
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    Serial.println(String(temp));
    timer = 0;
  }

  delay(100);
  timer += 0.1;
  
}
