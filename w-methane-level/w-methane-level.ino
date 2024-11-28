#include "DHT.h"

// Constants and pin definitions for the ultrasonic sensor and LEDs
#define TRIG_PIN 27  // Ultrasonic sensor trigger pin
#define ECHO_PIN 26  // Ultrasonic sensor echo pin
#define SPILL_LED 25 // Spill LED pin
#define WARN_LED 33  // Warning LED pin

// Constants for ultrasonic sensor distance thresholds
const float MAX_DISTANCE = 9.0;  // Maximum detectable distance in inches
const float MIN_DISTANCE = 0.2;  // Minimum detectable distance in inches
const float SPILL_THRESHOLD = 3.5;  // Spill threshold distance in inches
const float WARN_THRESHOLD = 4.5;  // Warning threshold distance in inches

// Constants and pin definitions for MQ-2, buzzer, and fan
#define MQ2_PIN 34        // Analog pin connected to MQ-2 (using GPIO34 for analog input)
#define BUZZER_PIN 12     // Buzzer pin
#define FAN_PIN 13        // Fan pin
const int SAFE_LEVEL = 1000;       // Safe level in PPM
const int CAUTIONARY_LEVEL = 5000; // Cautionary level in PPM

// DHT22 sensor settings
#define DHTPIN 14         // Pin connected to DHT22 data pin
#define DHTTYPE DHT22     // Specify the DHT sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Ultrasonic sensor and LED setup
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(SPILL_LED, OUTPUT);
  pinMode(WARN_LED, OUTPUT);

  // MQ-2, buzzer, and fan setup
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);

  // Turn off LEDs initially
  digitalWrite(SPILL_LED, LOW);
  digitalWrite(WARN_LED, LOW);

  // Initialize DHT sensor
  dht.begin();
  Serial.println("Initializing DHT sensor...");
}

void loop() {
  // Measure distance using ultrasonic sensor
  float distance = measureDistance();

  // Ensure distance is within sensor's measurable range
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
    // Determine the percentage of trash level
    float percentage = ((MAX_DISTANCE - distance) / MAX_DISTANCE) * 100;

    // Display status and control LEDs
    if (distance >= MIN_DISTANCE && distance <= SPILL_THRESHOLD) {
      // Spilling Level
      digitalWrite(SPILL_LED, HIGH);
      digitalWrite(WARN_LED, LOW);
      Serial.print("Distance: ");
      Serial.print(distance, 1);
      Serial.print(" inches | Percentage: ");
      Serial.print(percentage, 1);
      Serial.println("% | Status: Spilling Level");
    } else if (distance > SPILL_THRESHOLD && distance <= WARN_THRESHOLD) {
      // Warning Level
      digitalWrite(SPILL_LED, LOW);
      digitalWrite(WARN_LED, HIGH);
      Serial.print("Distance: ");
      Serial.print(distance, 1);
      Serial.print(" inches | Percentage: ");
      Serial.print(percentage, 1);
      Serial.println("% | Status: Warning Level");
    } else {
      // Safe Level
      digitalWrite(SPILL_LED, LOW);
      digitalWrite(WARN_LED, LOW);
      Serial.print("Distance: ");
      Serial.print(distance, 1);
      Serial.print(" inches | Percentage: ");
      Serial.print(percentage, 1);
      Serial.println("% | Status: Safe Level");
    }
  } else {
    // If distance is out of valid range
    digitalWrite(SPILL_LED, LOW);
    digitalWrite(WARN_LED, LOW);
    Serial.println("Distance out of range!");
  }

  // Read methane level from MQ-2
  int methaneLevel = analogRead(MQ2_PIN);
  float methanePercentage = (float)methaneLevel / 1023.0 * 100.0;

  // MQ-2 logic for methane levels
  if (methaneLevel < SAFE_LEVEL) {
    // Safe Level
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(FAN_PIN, LOW);
    Serial.print("Methane Level: ");
    Serial.print(methaneLevel);
    Serial.print(" PPM | Status: Safe Level | ");
    Serial.print(methanePercentage, 2);
    Serial.print("% | Fan: OFF");
    Serial.println();
  } else if (methaneLevel >= SAFE_LEVEL && methaneLevel < CAUTIONARY_LEVEL) {
    // Cautionary Level
    digitalWrite(BUZZER_PIN, HIGH); // Blinking buzzer
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    digitalWrite(FAN_PIN, HIGH); // Fan ON
    Serial.print("Methane Level: ");
    Serial.print(methaneLevel);
    Serial.print(" PPM | Status: Cautionary Level | ");
    Serial.print(methanePercentage, 2);
    Serial.print("% | Fan: ON");
    Serial.println();
  } else {
    // Hazardous Level
    digitalWrite(BUZZER_PIN, HIGH); // Solid buzzer sound
    digitalWrite(FAN_PIN, HIGH);   // Fan ON
    Serial.print("Methane Level: ");
    Serial.print(methaneLevel);
    Serial.print(" PPM | Status: Hazardous Level | ");
    Serial.print(methanePercentage, 2);
    Serial.print("% | Fan: ON");
    Serial.println();
  }

  // Read temperature and humidity from DHT22 with retry logic
  float temperature = NAN, humidity = NAN;
  int retries = 0;

  while (isnan(temperature) || isnan(humidity)) {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    if (++retries > 5) {
      Serial.println("Failed to read from DHT after retries!");
      break;
    }
    delay(100); // Retry delay
  }

  // Check if readings are valid
  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temperature: ");
    Serial.print(temperature, 1);
    Serial.print(" °C | Humidity: ");
    Serial.print(humidity, 1);
    Serial.println(" %");
  }

  delay(500); // Delay for stability
}

// Function to measure distance using the ultrasonic sensor
float measureDistance() {
  // Send a 10µs pulse to TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the duration of the echo signal with a timeout
  float duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout (covers max range of ~9 inches)

  // If no echo is detected, return a high value
  if (duration == 0) {
    return MAX_DISTANCE + 1; // Out of range
  }

  // Convert duration to distance in inches
  float distance = duration * 0.0133; // Speed of sound: 343 m/s = 0.0133 inches/µs

  return distance;
}
