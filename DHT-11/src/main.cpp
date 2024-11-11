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

unsigned long previousMillis = 0;
const long interval = 2000;


void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();
  if(BTSerial.available()){
    String command = BTSerial.readString();
    command.trim();

    if (command =="ON"){
      digitalWrite(RELAY_PIN,LOW);
      Serial.println("수동으로 물 공급 시작.");
      BTSerial.println("수동으로 물 공급 시작.");
    }else if(command =="OFF"){
      digitalWrite(RELAY_PIN,HIGH);
      Serial.println("수동으로 물 공급 중지.");
      BTSerial.println("수동으로 물 공급 중지.");
    }
    else if(command.startsWith("THRESHOLD")){
      int newThreshold = command.substring(9).toInt();
      thresholdMoisture = newThreshold;
      Serial.print("새 토양 습도 기준값: ");
      Serial.println(thresholdMoisture);
      BTSerial.print("새 토양 습도 기준값: ");
      BTSerial.println(thresholdMoisture);

    }
    
    
  }

  if(currentMillis-previousMillis>=interval){
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);

  if(isnan(humidity)||isnan(temperature)){
    Serial.println("DHT 센서 오류: 데이터를 읽을 수 없습니다.");
    BTSerial.println("DHT 센서 오류");
  }else{
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

  }

  

  if(soilMoistureValue > thresholdMoisture){
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("물을 공급합니다.");
    BTSerial.println("물을 공급합니다.");
  }else{
    digitalWrite(RELAY_PIN, HIGH);
    Serial.println("물 공급 중지.");
    BTSerial.println("물 공급 중지");
  }
  }
  

}

