#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_AUTH_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define VOLTAGE_PIN 34

char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_INA219 ina219;

BlynkTimer timer;

float voltage;
float current;
float power;

String mode;

void sendData() {

  int adc = analogRead(VOLTAGE_PIN);
  float adcVolt = (adc / 4095.0) * 3.3;
  voltage = adcVolt * 3.0;

  current = ina219.getCurrent_mA();

  if (current < 0) {
    current = -current;
  }

  power = voltage * (current / 1000.0);

  if (current < 20) {
    mode = "Plug in Mobile";
  }
  else if (voltage > 7.0) {
    mode = "Fast Charging";
  }
  else {
    mode = "Slow Charging";
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("V: ");
  display.print(voltage);
  display.print(" V");

  display.setCursor(0, 15);
  display.print("I: ");
  display.print(current);
  display.print(" mA");

  display.setCursor(0, 30);
  display.print("P: ");
  display.print(power);
  display.print(" W");

  display.setCursor(0, 45);
  display.print("Mode: ");
  display.print(mode);

  display.display();

  Blynk.virtualWrite(V4, voltage);
  Blynk.virtualWrite(V8, current);
  Blynk.virtualWrite(V6, power);
  Blynk.virtualWrite(V7, mode);

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | ");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" mA | ");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.print(" W | ");

  Serial.print("Mode: ");
  Serial.println(mode);
}

void setup() {

  Serial.begin(115200);

  Wire.begin(21, 22);

  analogReadResolution(12);
  analogSetPinAttenuation(VOLTAGE_PIN, ADC_11db);

  if (!ina219.begin()) {
    Serial.println("INA219 not found");
    while (1);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while (1);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("System Ready");
  display.display();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(5000L, sendData);
}

void loop() {
  Blynk.run();
  timer.run();
}
