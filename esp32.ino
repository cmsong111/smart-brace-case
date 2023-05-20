#include "BluetoothSerial.h"

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

// LED on GP102
int receive_LedPin = 22;  // 수신 시 점등 LED
int send_LedPin = 23;     // 송신 시 점등 LED
int IR_Sensor = 36;
int IR_Value = 0;
char str1[] = {  '{', '"', 's', 't', 'a', 't', 'u', 's', '"', ':', 't', 'r', 'u', 'e', '}' };

void setup() {
  pinMode(send_LedPin, OUTPUT);
  pinMode(receive_LedPin, OUTPUT);
  pinMode(IR_Sensor, INPUT);
  Serial.begin(115200);

  SerialBT.begin(device_name);  // Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());

  //Serial.printf("The device with name \"%s\" and MAC address %s is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str(), SerialBT.getMacString()); // Use this after the MAC method is implemented

#ifdef USE_PIN
  SerialBT.setPin(pin);
  Serial.println("Using PIN");
#endif
}

void loop() {
  IR_Value = digitalRead(IR_Sensor);

  if (IR_Value == HIGH) {

    SerialBT.write(65);

    Serial.println("[적외선 센서] 물체 감지함!!");
  } else {

    for (int i = 0; i < strlen(str1); i++) {
      SerialBT.write(str1[i]);
    }

    SerialBT.write(13);
    SerialBT.write(10);

    Serial.println("[적외선 센서] 감지되는 물체 없음...");
  }

  if (Serial.available()) {  // Serial 값 송신 했을 때
    String input = Serial.readString();
    SerialBT.print(input);
    digitalWrite(receive_LedPin, HIGH);
    digitalWrite(send_LedPin, LOW);
  }

  if (SerialBT.available()) {  // Serial 값 수신 했을 때
    String input = SerialBT.readString();
    Serial.print(input);
    digitalWrite(send_LedPin, HIGH);
    digitalWrite(receive_LedPin, LOW);
  }

  delay(1000);
}
