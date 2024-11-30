#include <WiFi.h>
#include "DHT.h"
#include "index.html.h" // Include the HTML header

// WiFi Credentials
const char *ssid = "ASY";
const char *password = "ysabela18";

// WiFi Server
WiFiServer server(80);

// DHT Sensor Configuration
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonic Sensor and LEDs
#define TRIG_PIN 27
#define ECHO_PIN 26
#define SPILL_LED 25
#define WARN_LED 33

// MQ-2, Buzzer, and Fan
#define MQ2_PIN 34
#define BUZZER_PIN 12
#define FAN_PIN 13

// Thresholds
const float MAX_DISTANCE = 9.0;
const float MIN_DISTANCE = 0.2;
const float SPILL_THRESHOLD = 3.5;
const float WARN_THRESHOLD = 4.5;
const int SAFE_LEVEL = 1000;
const int CAUTIONARY_LEVEL = 5000;

void setup() {
  Serial.begin(115200);

  // WiFi Connection
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start Server
  server.begin();

  // Initialize Sensors and Actuators
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SPILL_LED, OUTPUT);
  pinMode(WARN_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(SPILL_LED, LOW);
  digitalWrite(WARN_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  dht.begin();
}

void loop() {
  // Accept incoming client
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client connected.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        // Check for end of request
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // HTTP Response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // Generate dynamic content
            String page = index_html;
            page.replace("%DISTANCE%", String(measureDistance()));
            int methaneLevel = analogRead(MQ2_PIN);
            page.replace("%METHANE_LEVEL%", String(methaneLevel));
            page.replace("%METHANE_PERCENTAGE%", String((float)methaneLevel / 1023.0 * 100.0));
            float temperature = dht.readTemperature();
            float humidity = dht.readHumidity();
            page.replace("%TEMPERATURE%", isnan(temperature) ? "N/A" : String(temperature));
            page.replace("%HUMIDITY%", isnan(humidity) ? "N/A" : String(humidity));
            page.replace("%LED_STATUS%", digitalRead(SPILL_LED) ? "SPILL" : (digitalRead(WARN_LED) ? "WARN" : "SAFE"));
            page.replace("%BUZZER_STATUS%", digitalRead(BUZZER_PIN) ? "ON" : "OFF");
            page.replace("%FAN_STATUS%", digitalRead(FAN_PIN) ? "ON" : "OFF");

            // Send the generated page
            client.println(page);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    // Close connection
    client.stop();
    Serial.println("Client disconnected.");
  }

  // Regular sensor logic
  handleSensors();
}

void handleSensors() {
  float distance = measureDistance();

  // Ultrasonic sensor logic
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
    if (distance <= SPILL_THRESHOLD) {
      digitalWrite(SPILL_LED, HIGH);
      digitalWrite(WARN_LED, LOW);
    } else if (distance <= WARN_THRESHOLD) {
      digitalWrite(SPILL_LED, LOW);
      digitalWrite(WARN_LED, HIGH);
    } else {
      digitalWrite(SPILL_LED, LOW);
      digitalWrite(WARN_LED, LOW);
    }
  }

  // MQ-2 logic
  int methaneLevel = analogRead(MQ2_PIN);
  if (methaneLevel < SAFE_LEVEL) {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
  } else if (methaneLevel < CAUTIONARY_LEVEL) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(FAN_PIN, HIGH);
  }
}

float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  float duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return MAX_DISTANCE + 1;
  return duration * 0.0133; // Convert to inches
}
