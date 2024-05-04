#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6br_bcayg"
#define BLYNK_TEMPLATE_NAME "LED ESP32"
#define BLYNK_AUTH_TOKEN "_zzMSzpW7zQ14CiSLCnqpQUCjWDs48Qj"

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int voltage_offset = 20;  // set the correction offset value

char ssid[] = "Jake Wifi";
char pass[] = "Aabbcc_12345";

void setup() {
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(CURRENT_PIN, INPUT);
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
  voltage /= 100;                       // divide by 100 to get the decimal values
  double voltageFinal = voltage + 0.5;  //offset
  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.println("V");
  Blynk.virtualWrite(V1, voltageFinal);
  delay(500);
  return voltageFinal;
}

double readCurrentSensor() {
  int adc = analogRead(CURRENT_PIN);
  float R1 = 6800.0;
  float R2 = 12000.0;
  float adc_voltage = adc * (3.3 / 4096.0);
  float current_voltage = (adc_voltage * (R1 + R2) / R2);
  float current = (current_voltage - 2.5) / 0.100;
  Serial.print("Current Value: ");
  Serial.println(current);
  Blynk.virtualWrite(V2, current);
  delay(500);
  return current;
}

double calculatePower() {
  double power = readVoltageSensor() * readCurrentSensor();
  Blynk.virtualWrite(V3, power);
  return power;
}
