#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Wi-Fi credentials
const char* ssid = "Redmi 9C"; // Replace with your Wi-Fi SSID
const char* password = "correa123"; // Replace with your Wi-Fi password

// Web server on port 80
WebServer server(80);

// Constants and pin definitions for the ultrasonic sensor and LEDs
#define TRIG_PIN 27
#define ECHO_PIN 26
#define SPILL_LED 33
#define WARN_LED 25

// Constants for ultrasonic sensor distance thresholds
const float MAX_DISTANCE = 21.0;
const float MIN_DISTANCE = 0.2;
const float SPILL_THRESHOLD = 2;  // Spill threshold distance in inches
const float WARN_THRESHOLD = 11;  // Warning threshold distance in inches

// Constants and pin definitions for MQ-2, buzzer, and fan
#define MQ2_PIN 34
#define BUZZER_PIN 12
#define FAN_PIN 13
const int SAFE_LEVEL = 1000;       // Safe level in PPM
const int CAUTIONARY_LEVEL = 5000; // Cautionary level in PPM


// DHT22 sensor settings
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Initialize components
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

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Configure web server
  server.on("/", handleRoot);        // Main webpage
  server.on("/data", handleData);    // JSON data
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Measure the waste level (distance)
  float distance = measureDistance();
  
  // Measure methane level
  int methaneLevel = analogRead(MQ2_PIN);

  // Handle methane levels
  if (methaneLevel < SAFE_LEVEL) {
    // Safe Level
    
    digitalWrite(BUZZER_PIN, LOW); // Ensure buzzer is off
  } else if (methaneLevel >= SAFE_LEVEL && methaneLevel < CAUTIONARY_LEVEL) {
    // Cautionary Level
  
    
    // Sound buzzer 5 times with a fast delay
    for (int i = 0; i < 5; i++) {
      digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
      delay(100);                      // Fast delay
      digitalWrite(BUZZER_PIN, LOW);   // Turn off buzzer
      delay(100);                      // Fast delay
    }
  } else {
    // Hazardous Level
    
    // Continuously sound buzzer (blinking sound)
    while (methaneLevel >= CAUTIONARY_LEVEL) {
      digitalWrite(BUZZER_PIN, HIGH);  // Turn on buzzer
      delay(500);                      // Buzzer on for 500 ms
      digitalWrite(BUZZER_PIN, LOW);   // Turn off buzzer
      delay(500);                      // Buzzer off for 500 ms
      
      // Re-check methane level to ensure it's still in the hazardous range
      methaneLevel = analogRead(MQ2_PIN);
    }
  }

  // Control the LEDs based on the waste level
  if (distance >= MIN_DISTANCE && distance <= SPILL_THRESHOLD) {
    digitalWrite(SPILL_LED, HIGH); // Red LED lights
    digitalWrite(WARN_LED, LOW);   // Yellow LED off
  } else if (distance > SPILL_THRESHOLD && distance <= WARN_THRESHOLD) {
    digitalWrite(SPILL_LED, LOW);  // Red LED off
    digitalWrite(WARN_LED, HIGH);  // Yellow LED lights
  } else {
    digitalWrite(SPILL_LED, LOW);  // Red LED off
    digitalWrite(WARN_LED, LOW);   // Yellow LED off
  }

  // Handle incoming client requests
  server.handleClient();
}


// Function to handle the root webpage
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>GreenSense Dashboard</title>
    <style>
        /* Your existing CSS */
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            background-color: #fbfaf7;
            color: #222e40;
            overflow: hidden;
        }
        .title {
            position: absolute;
            top: 5px;
            left: 25%;
            transform: translateX(-50%);
            text-align: center;
            z-index: 1;
        }
        .title h1 {
            font-family: 'League Gothic', sans-serif;
            color: #009950;
            font-size: 110px;
            font-weight: bold;
            margin: 0;
            font-style: italic;
        }
        .title p {
            color: #00bf63;
            font-size: 19px;
            margin-top: -10px;
            text-align: start;
            margin-left: 15px;
        }
        .logo {
            width: 8%;
            position: absolute;
            height: auto;
            left: 60px;
            top: 27px;
        }
        .container {
            max-width: 1200px;
            margin: auto;
            padding-top: 200px;
        }
        .dashboard {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
            gap: 20px;
        }
        .card {
            background: #fff5e7;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            align-items: center;
            text-align: center;
            height: 195px;
        }
        .card1 {
            width: 90%;
            height: 195px;
            background: #fff5e7;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            align-items: center;
            text-align: center;
        }
        .card2 {
            background: #fff5e7;
            padding: 20px;
            box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
            border-radius: 10px;
            display: flex;
            flex-direction: column;
            justify-content: space-between;
            align-items: center;
            text-align: center;
            height: 195px;
            position: absolute;
            width: 22.5%;
            margin: 250px 0px 0px 408px;
        }
        .status-text {
            margin-top: 10px;
            font-size: 1.3rem;
            color: #000;
            margin-right: 275px;
        }
        #trashLevelText, #temperatureText, #ppm-text, #humidityText {
            font-size: 3.5rem;
            margin-top: 25px;
            font-weight: bold;
            color: #222e40;
        }
    </style>
    <script>
        async function fetchData() {
            try {
                const response = await fetch('/data');
                const data = await response.json();

                // Update the HTML elements with the fetched data
                document.getElementById('trashLevelText').textContent = `${(data.distance).toFixed(1)} cm`;
                document.getElementById('temperatureText').textContent = data.temperature !== null ? `${data.temperature.toFixed(1)} °C` : "--°C";
                document.getElementById('ppm-text').textContent = `${data.methaneLevel} ppm`;
                document.getElementById('humidityText').textContent = data.humidity !== null ? `${data.humidity.toFixed(1)}%` : "--%";
            } catch (error) {
                console.error('Error fetching data:', error);
            }
        }

        // Refresh the data every 2 seconds
        setInterval(fetchData, 2000);

        // Fetch data once on page load
        document.addEventListener('DOMContentLoaded', fetchData);
    </script>
</head>
<body>
    
    <div class="container">
        <img src="recycle.png" alt="Recycle Logo" class="logo" style="display:none">
        <div class="title">
            <h1>GREENSENSE</h1>
            <p>Design and Development of Sustainable Waste
               Management through Internet of Things</p>
        </div>
        <div class="dashboard">
            <div class="card1">
                <h2>Waste Level</h2>
                <p id="trashLevelText">--%</p>
                
            </div>
            <div class="card">
                <h2>Temperature Level (°C)</h2>
                <p id="temperatureText">--°C</p>
                
            </div>
            <div class="card">
                <h2>Methane Level (PPM)</h2>
                <p id="ppm-text">----.--ppm</p>
                
            </div>
            <div class="card2">
                <h2>Humidity (%)</h2>
                <p id="humidityText">--%</p>
                
            </div>
        </div>
    </div>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}


// Function to serve sensor data in JSON format
void handleData() {
  float distance = measureDistance();
  int methaneLevel = analogRead(MQ2_PIN);
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // JSON structure for the data
  String json = "{";
  json += "\"distance\":" + String(distance, 1) + ",";
  json += "\"methaneLevel\":" + String(methaneLevel) + ",";
  if (!isnan(temperature) && !isnan(humidity)) {
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1);
  } else {
    json += "\"temperature\":null,\"humidity\":null";
  }
  json += "}";

  server.send(200, "application/json", json);
}

// Function to measure distance using the ultrasonic sensor
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  float duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) {
    return MAX_DISTANCE + 1; // Return max distance if no echo is received
  }
  return duration * 0.0133; // Convert the pulse duration to distance in cm
}
