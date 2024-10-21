#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN 7

int soilMoistureValue = 0;
int thresholdMoisture = 600;
float humidity = 0;
float temperature = 0;


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

  Serial.print("온도: ");
  Serial.print(temperature);
  Serial.print("°C 습도: ");
  Serial.print(humidity);
  Serial.print("% 토양 습도: ");
  Serial.print(soilMoistureValue);

  if(soilMoistureValue > thresholdMoisture){
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("물을 공급합니다.");
  }else{
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("물 공급 중지.");
  }

  delay(2000);

}

