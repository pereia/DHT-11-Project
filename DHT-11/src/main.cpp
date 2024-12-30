#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// 핀 설정
#define DHTPIN 2           // DHT11 데이터 핀
#define DHTTYPE DHT11      // DHT 센서 유형
#define SOIL_MOISTURE_PIN A0   // 토양 습도 센서 핀
#define RELAY_PIN 7        // 릴레이 제어 핀

// 블루투스 통신 핀 설정
SoftwareSerial BTSerial(10, 11); // RX = 10, TX = 11

// DHT 센서 초기화
DHT dht(DHTPIN, DHTTYPE);

// 전역 변수
int soilMoistureValue = 0;      // 토양 습도 값
int thresholdMoisture = 600;    // 기본 토양 습도 임계값
float humidity = 0;             // 습도 값
float temperature = 0;          // 온도 값
bool manualControl = false;     // 수동 제어 활성화 여부

// 시간 관리
unsigned long previousMillis = 0;
const long interval = 2000;     // 2초 간격

// 토양 수분 값 읽기 함수
int readSoilMoisture() {
  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(SOIL_MOISTURE_PIN);
    delay(10);
  }
  return total / 10; // 평균값 반환
}

void setup() {
  Serial.begin(9600);        // 시리얼 통신 초기화
  BTSerial.begin(9600);      // 블루투스 통신 초기화
  dht.begin();               // DHT 센서 시작
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // 릴레이 OFF 상태로 시작

  Serial.println("시스템 초기화 완료");
}

void loop() {
  unsigned long currentMillis = millis();

  // **블루투스 명령 처리**
  if (BTSerial.available()) {
    String command = BTSerial.readString();
    command.trim();

    if (command == "ON") { // 펌프 수동 ON
      manualControl = true; // 수동 모드 활성화
      digitalWrite(RELAY_PIN, LOW); // 물 공급
      Serial.println("수동으로 물 공급 시작.");
      BTSerial.println("수동으로 물 공급 시작.");
    } else if (command == "OFF") { // 펌프 수동 OFF
      manualControl = true; // 수동 모드 활성화
      digitalWrite(RELAY_PIN, HIGH); // 물 공급 중단
      Serial.println("수동으로 물 공급 중지.");
      BTSerial.println("수동으로 물 공급 중지.");
    } else if (command.startsWith("THRESHOLD")) { // 임계값 설정
      int newThreshold = command.substring(9).toInt();
      if (newThreshold >= 0 && newThreshold <= 1023) { // 유효한 값 확인
        thresholdMoisture = newThreshold;
        Serial.print("새 토양 습도 기준값: ");
        Serial.println(thresholdMoisture);
        BTSerial.print("새 토양 습도 기준값: ");
        BTSerial.println(thresholdMoisture);
      } else {
        Serial.println("오류: 유효한 임계값 범위는 0~1023입니다.");
        BTSerial.println("오류: 유효한 임계값 범위는 0~1023입니다.");
      }
    } else if (command == "AUTO") { // 자동 모드 활성화
      manualControl = false; // 자동 모드 활성화
      Serial.println("자동 모드 활성화.");
      BTSerial.println("자동 모드 활성화.");
    }
  }

  // **자동 모드: 센서 값 기반 제어**
  if (!manualControl && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 토양 수분 센서 값 읽기
    soilMoistureValue = readSoilMoisture();
    Serial.print("토양 습도 값: ");
    Serial.println(soilMoistureValue);

    if (soilMoistureValue > thresholdMoisture) { // 건조하면 물 공급
      digitalWrite(RELAY_PIN, LOW);
      Serial.println("물을 공급합니다.");
      BTSerial.println("물을 공급합니다.");
    } else { // 습하면 물 공급 중단
      digitalWrite(RELAY_PIN, HIGH);
      Serial.println("물 공급 중지.");
      BTSerial.println("물 공급 중지.");
    }

    // **DHT 센서 값 출력**
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      Serial.print("온도: ");
      Serial.print(temperature);
      Serial.print("°C 습도: ");
      Serial.print(humidity);
      Serial.println("%");

      BTSerial.print("온도: ");
      BTSerial.print(temperature);
      BTSerial.print("°C 습도: ");
      BTSerial.print(humidity);
      BTSerial.println("%");
    } else {
      Serial.println("DHT 센서 오류: 데이터를 읽을 수 없습니다.");
      BTSerial.println("DHT 센서 오류");
    }
  }
}
