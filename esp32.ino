#include "BluetoothSerial.h"
#include <ESP32Time.h>
#include <ArduinoJson.h>


//#define USE_PIN // Uncomment this to use PIN during pairing. The pin is specified on the line below
const char *pin = "1234";  // Change this to a more secure PIN.

String device_name = "굴리오 ESP32";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;
DynamicJsonDocument doc(1024);
DynamicJsonDocument doc2(1024);
ESP32Time rtc(3600 * 9);


// LED on GP102
int receive_LedPin = 22;  // 수신 시 점등 LED
int send_LedPin = 23;     // 송신 시 점등 LED
int IR_Sensor = 36;
int IR_Value = 0;

void sendMessage(String message) {
  // Json 메시지 전송
  for (int i = 0; i < message.length(); i++) {
    SerialBT.write(message[i]);
  }
  // 버퍼 비우기
  SerialBT.flush();

  // 개행문자 추가
  SerialBT.write(13);
  SerialBT.write(10);
}

void setup() {
  pinMode(send_LedPin, OUTPUT);
  pinMode(receive_LedPin, OUTPUT);
  pinMode(IR_Sensor, INPUT);
  Serial.begin(115200);

  SerialBT.begin(device_name);  // Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented
  rtc.setTime(30, 24, 15, 17, 1, 2021);

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
  doc["status"] = true;
#endif
}

void setTime(int hour, int minute, int second, int day, int month, int year) {
  rtc.setTime(hour, minute, second, day, month, year);
}

void loop() {
  String output;

  IR_Value = digitalRead(IR_Sensor);
  doc["time"] = rtc.getTime("%Y-%m-%d %H:%M:%S");

  if (IR_Value == HIGH) {
    doc["status"] = true;
    Serial.println("[적외선 센서] 물체 감지함!!");
  } else {
    doc["status"] = false;
    Serial.println("[적외선 센서] 감지되는 물체 없음...");
  }

  serializeJson(doc, output);
  sendMessage(output);

  if (Serial.available()) {  // Serial 값 송신 했을 때
    String input = Serial.readString();
    SerialBT.print(input);
    digitalWrite(receive_LedPin, HIGH);
    digitalWrite(send_LedPin, LOW);
  }

  if (SerialBT.available()) {  // Serial 값 수신 했을 때
    String input = SerialBT.readString();
    Serial.print(input);

    deserializeJson(doc2, input);

    int hour = doc2["hour"];
    int minute = doc2["minute"];
    int second = doc2["second"];
    int day = doc2["day"];
    int month = doc2["month"];
    int year = doc2["year"];

    setTime(hour, minute, second, day, month, year);

    digitalWrite(send_LedPin, HIGH);
    digitalWrite(receive_LedPin, LOW);
  }

  delay(2500);
}
