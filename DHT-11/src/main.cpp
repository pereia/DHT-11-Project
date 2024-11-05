#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>;

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN 7

SoftwareSerial BTSerial(10,11);

int soilMoistureValue = 0;
int thresholdMoisture = 600;
float humidity = 0;
float temperature = 0;


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
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

  BTSerial.print("온도: ");
  BTSerial.print(temperature);
  BTSerial.print("°C 습도: ");
  BTSerial.print(humidity);
  BTSerial.print("% 토양 습도: ");
  BTSerial.print(soilMoistureValue);

  if(soilMoistureValue > thresholdMoisture){
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("물을 공급합니다.");
    BTSerial.println("물을 공급합니다.");
  }else{
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("물 공급 중지.");
    BTSerial.println("물 공급 중지");
  }

  delay(2000);

}

