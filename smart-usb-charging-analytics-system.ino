#define BLYNK_TEMPLATE_ID "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "YOUR_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "YOUR_BLYNK_TOKEN"

#define BLYNK_PRINT Serial

#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <HTTPClient.h>
#include <Adafruit_INA219.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

/* ---------------- OLED Configuration ---------------- */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/* ---------------- Sensor Pin ---------------- */
#define VOLTAGE_PIN 34

/* ---------------- WiFi Credentials ---------------- */
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

/* ---------------- Google Sheets Web App URL ---------------- */
String sheetURL = "YOUR_GOOGLE_SCRIPT_URL";

/* ---------------- Device Objects ---------------- */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_INA219 ina219;

/* ---------------- Measurement Variables ---------------- */
float voltage = 0;
float current = 0;
float power = 0;
float energy_Wh = 0;

String chargingMode;

/* ---------------- Timing ---------------- */
unsigned long lastTime = 0;
const unsigned long interval = 5000;

/* =========================================================
   Function: Read Sensors and Process Data
   ========================================================= */
void readSensors() {

  int adc = analogRead(VOLTAGE_PIN);
  float adcVolt = (adc / 4095.0) * 3.3;

  voltage = adcVolt * 3.0;

  current = abs(ina219.getCurrent_mA());

  power = voltage * (current / 1000.0);

  energy_Wh += power * (interval / 1000.0 / 3600.0);
}

/* =========================================================
   Function: Detect Charging Mode
   ========================================================= */
void detectChargingMode() {

  if (current < 20) {
    chargingMode = "Plug in Mobile";
  }
  else if (voltage > 7.0 && current > 800) {
    chargingMode = "Fast Charging";
  }
  else if (voltage > 7.0 && current <= 800) {
    chargingMode = "Taper Charging";
  }
  else {
    chargingMode = "Slow Charging";
  }
}

/* =========================================================
   Function: Update OLED Display
   ========================================================= */
void updateOLED() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.print("V: ");
  display.print(voltage);

  display.setCursor(0, 12);
  display.print("I: ");
  display.print(current);

  display.setCursor(0, 24);
  display.print("P: ");
  display.print(power);

  display.setCursor(0, 36);
  display.print("E: ");
  display.print(energy_Wh);

  display.setCursor(0, 48);
  display.print(chargingMode);

  display.display();
}

/* =========================================================
   Function: Send Data to Blynk
   ========================================================= */
void sendToBlynk() {

  Blynk.virtualWrite(V4, voltage);
  Blynk.virtualWrite(V8, current);
  Blynk.virtualWrite(V6, power);
  Blynk.virtualWrite(V7, chargingMode);
  Blynk.virtualWrite(V9, energy_Wh);
}

/* =========================================================
   Function: Send Data to Google Sheets
   ========================================================= */
void sendToGoogleSheets() {

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  String safeMode = chargingMode;
  safeMode.replace(" ", "%20");

  String url = sheetURL + "?voltage=" + String(voltage) +
               "&current=" + String(current) +
               "&power=" + String(power) +
               "&mode=" + safeMode;

  Serial.println(url);

  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.begin(client, url);

  int httpCode = http.GET();

  Serial.print("HTTP Response: ");
  Serial.println(httpCode);

  http.end();
}

/* =========================================================
   Function: Serial Monitor Output
   ========================================================= */
void printSerial() {

  Serial.print("Voltage: ");
  Serial.print(voltage);
  Serial.print(" V | ");

  Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" mA | ");

  Serial.print("Power: ");
  Serial.print(power);
  Serial.print(" W | ");

  Serial.print("Energy: ");
  Serial.print(energy_Wh);
  Serial.print(" Wh | ");

  Serial.print("Mode: ");
  Serial.println(chargingMode);
}

/* =========================================================
   Setup
   ========================================================= */
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

  Serial.println("System Connected");
}

/* =========================================================
   Main Loop
   ========================================================= */
void loop() {

  Blynk.run();

  if (millis() - lastTime > interval) {

    readSensors();
    detectChargingMode();
    updateOLED();
    sendToBlynk();
    sendToGoogleSheets();
    printSerial();

    lastTime = millis();
  }
}
