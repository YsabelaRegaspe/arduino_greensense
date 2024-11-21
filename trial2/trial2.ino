#include <WiFi.h> // Use <ESP8266WiFi.h> for ESP8266
#include "index.html.h" // Include the HTML file

// Replace with your network credentials
const char* ssid = "ASY";
const char* password = "ysabela18";

// Create a WiFiServer object
WiFiServer server(80);

// Define LED pin
const int ledPin = 2; // GPIO2 (D2) on ESP32
bool ledState = false; // Track LED state

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Ensure LED is off initially

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  Serial.println("New Client connected");
  String request = "";

  // Read HTTP request
  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      request += c;

      // If request ends with a newline, respond to the client
      if (c == '\n') {
        if (request.indexOf("GET /toggle") >= 0) {
          ledState = !ledState; // Toggle LED state
          digitalWrite(ledPin, ledState ? HIGH : LOW);
        } else if (request.indexOf("GET /status") >= 0) {
          // Respond with LED state
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type:text/plain");
          client.println();
          client.print(ledState ? "1" : "0");
          client.println();
          break;
        }

        // Serve the HTML page
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();
        client.print(index_html); // Serve the HTML page
        client.println();
        break;
      }
    }
  }

  // Close the connection
  client.stop();
  Serial.println("Client disconnected");
}
