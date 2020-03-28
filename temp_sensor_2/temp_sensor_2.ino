#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char* ssid     = "<Network name here>";
const char* password = "<Network password here>";
const char* target = "http://<IP Address of InfluxDB here>:8086/write?db=room_monitor";
const int interval = 10; // Read temperature every n seconds

float timer = interval * 1000;

void connect() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);   //WiFi connection
    while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
      delay(500);
      Serial.println("Waiting for connection");
    } 
  }
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  
  if (timer >= interval) {

    connect();
  
    HTTPClient http;
    OneWire oneWire(0);
    DallasTemperature sensors(&oneWire);
    sensors.begin();
  
    http.begin(target);
    http.addHeader("Content-Type", "text/plain");
  
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
 
    String payload = "room_temp value=" + String(temp);
    Serial.println(payload);
  
    int httpResponseCode = http.POST(payload);
    
    if (httpResponseCode > 0) {
   
      String response = http.getString();   
   
      Serial.println(httpResponseCode);
      Serial.println(response);
   
    } else {
  
      Serial.print("Error on sending POST Request: ");
      Serial.println(httpResponseCode);
   
    }
   
    http.end();
    Serial.println("Waiting " + String(interval) + " seconds");
    timer = 0;

  }

  delay(100);
  timer += 0.1;
  
}
