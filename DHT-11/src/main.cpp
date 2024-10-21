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
}

