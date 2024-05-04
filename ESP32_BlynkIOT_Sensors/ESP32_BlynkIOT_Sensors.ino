#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6br_bcayg"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "_zzMSzpW7zQ14CiSLCnqpQUCjWDs48Qj"

#define VOLTAGE_PIN 34

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int voltage_offset = 20;  // set the correction offset value

char ssid[] = "Jake Wifi";
char pass[] = "Aabbcc_12345";

void setup() {
  pinMode(VOLTAGE_PIN, INPUT);
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  calculatePower();
}

double readVoltageSensor() {
  int volt = analogRead(VOLTAGE_PIN);  // read the input - GPIO_NUM_34
  double voltage = map(volt, 0, 4096, 0, 1650) + voltage_offset;
  voltage /= 100;                // divide by 100 to get the decimal values
  double voltageFinal = voltage + 0.5;  //offset
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  Blynk.virtualWrite(V1, voltageFinal);
  delay(500);
  return voltageFinal;
}

double readCurrentSensor() {
  double current = 10.5;

  Blynk.virtualWrite(V2, current);
  return current;
}

double calculatePower() {
  double power = readVoltageSensor() * readCurrentSensor();
  Blynk.virtualWrite(V3, power);
  return power;
}