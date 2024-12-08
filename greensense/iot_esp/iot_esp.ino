#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "DHT.h"

// Constants and pin definitions
#define TRIG_PIN 27
#define ECHO_PIN 26
#define DHTPIN 14
#define DHTTYPE DHT22
#define ON_Board_LED 2
#define caution_led 25
#define warn_led 33
#define MQ2_PIN 34
#define BUZZER_PIN 12

// Distance thresholds (in inches)
const float MAX_DISTANCE = 9.0;
const float MIN_DISTANCE = 0.2;
const float SPILL_THRESHOLD = 3.5;
const float CAUTION_THRESHOLD = 4.5;

// MQ-2
const int SAFE_LEVEL = 1000;       // Safe level in PPM
const int CAUTIONARY_LEVEL = 5000; // Cautionary level in PPM

// WiFi credentials
const char* ssid = "SKYW_180B_2G";
const char* password = "UEFgP3X4";

// Global variables
float distance = 0.0;
float temperature = 0.0;
float humidity = 0.0;
float waste_percent = 0.0;
String send_waste_status = "";
String send_tempt_status = "";
String send_humidity_status = "";
String send_methane_status = "";
String postData = "";
String payload = "";
String send_Status_Read_DHT22 = "";
float send_Temp = 0.0;
int methaneLevel = 0;

DHT dht22_sensor(DHTPIN, DHTTYPE);

// Measure distance using the ultrasonic sensor
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.0133; // Convert to inches
  return distance;
}

// Process ultrasonic sensor data
void get_ultrasonic_sensor_data() {
  distance = measureDistance();
  waste_percent = ((MAX_DISTANCE - distance) / MAX_DISTANCE) * 100;
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
    if (distance <= SPILL_THRESHOLD) {
      send_waste_status = "Warning Level";
      Serial.printf("Level: %.2f%%, Status: %s\n", waste_percent, send_waste_status.c_str());
      digitalWrite(warn_led, HIGH);
      digitalWrite(caution_led, LOW);
    } else if (distance <= CAUTION_THRESHOLD) {
      send_waste_status = "Caution Level";
      Serial.printf("Level: %.2f%%, Status: %s\n", waste_percent, send_waste_status.c_str());
      digitalWrite(warn_led, LOW);
      digitalWrite(caution_led, HIGH);
    } else {
      send_waste_status = "Safe Level";
      Serial.printf("Level: %.2f%%, Status: %s\n", waste_percent, send_waste_status.c_str());
      digitalWrite(warn_led, LOW);
      digitalWrite(caution_led, LOW);
    }
  } else {
    Serial.println("Distance out of range!");
  }
}

// Process DHT22 sensor data
void get_DHT22_sensor_data() {
  temperature = dht22_sensor.readTemperature();
  humidity = dht22_sensor.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_Temp = temperature;
  send_tempt_status = (temperature > 40) ? "Temperature Critical" :
                      (temperature > 35) ? "Temperature Caution" :
                                           "Temperature Normal";
  send_humidity_status = (humidity > 80) ? "High Humidity Warning" :
                        (humidity > 60) ? "Humidity Caution" :
                                           "Normal Humidity";

  Serial.printf("Temperature: %.2f°C, Status: %s\n", temperature, send_tempt_status.c_str());
  Serial.printf("Humidity: %.2f%%, Status: %s\n", humidity, send_humidity_status.c_str());
}

// Function to handle methane level logic and buzzer
void handleMethaneLevel() {
  methaneLevel = analogRead(MQ2_PIN);
  float methanePercentage = (float)methaneLevel / 1023.0 * 100.0;

  if (methaneLevel < SAFE_LEVEL) {
    send_methane_status = "Safe Level";
    digitalWrite(BUZZER_PIN, LOW);
  } else if (methaneLevel >= SAFE_LEVEL && methaneLevel < CAUTIONARY_LEVEL) {
    send_methane_status = "Cautionary Level";
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(100);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
    }
  } else {
    send_methane_status = "Critical Methane Alert";
    while (methaneLevel >= CAUTIONARY_LEVEL) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(500);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);
      methaneLevel = analogRead(MQ2_PIN);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ON_Board_LED, OUTPUT);
  pinMode(caution_led, OUTPUT);
  pinMode(warn_led, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  dht22_sensor.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("\nConnected to WiFi!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    int httpCode;
    String postData;
    String payload;

    // Get data from the server (getdata.php)
    Serial.println("---------------getdata.php");
    http.begin("http://192.168.1.63/esp32_testing/getdata.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    postData = "id=esp32_01"; // Send device identifier to fetch relevant data

    httpCode = http.POST(postData);
    payload = http.getString();

    if (httpCode > 0) {
      Serial.printf("HTTP Code (getdata.php): %d, Response: %s\n", httpCode, payload.c_str());
      // Parse the payload (optional, depending on server response format)
      // Use the payload data to control LEDs, etc.
    } else {
      Serial.printf("Error fetching data: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    // Process sensor data and send it to the server (updateDHT22data.php)
    Serial.println("---------------updateDHT22data.php");
    http.begin("http://192.168.1.63/esp32_testing/sensors_data.php");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    postData = "id=esp32_01";
    postData += "&status_read_sensor_dht22=" + send_waste_status;
    postData += "&waste_level_percent=" + String(waste_percent);
    postData += "&waste_status=" + send_waste_status;

    postData += "&temperature_celsius=" + String(temperature);
    postData += "&tempt_status=" + send_tempt_status;
    postData += "&humidity_percent=" + String(humidity);
    postData += "&humidity_status=" + send_humidity_status;
    postData += "&methane_level=" + String(methaneLevel);
    postData += "&methane_status=" + send_methane_status;

    httpCode = http.POST(postData);
    payload = http.getString();

    if (httpCode > 0) {
      Serial.printf("HTTP Code (updateDHT22data.php): %d, Response: %s\n", httpCode, payload.c_str());
    } else {
      Serial.printf("Error sending data: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();

    // Add a delay to prevent rapid requests
    delay(5000);
  } else {
    Serial.println("WiFi not connected. Retrying...");
    WiFi.reconnect();
    delay(5000);
  }
}
