#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Redmi 12 5G";
const char* password = "rakshith";

// Define the specified pins for the Ultrasonic Sensor (HC-SR04)
const int trigPin = 32; 
const int echoPin = 35;

// Define the dustbin parameters
const int maxDistanceCm = 25; // The total height/depth of the dustbin in cm. Adjust as needed.
const int minDistanceCm = 5;  // Distance when the bin is considered "Full" in cm. Adjust as needed.

// Speed of sound in cm/microseconds
#define SOUND_SPEED 0.034 

// Variable to store distance and duration
long duration;
float distanceCm;
int fillPercentage = 0;

// Create an instance of the server on port 80
WiFiServer server(80);

// Function to calculate distance
long measureDistance() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin HIGH for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  // Distance = (duration * speed of sound) / 2
  distanceCm = duration * SOUND_SPEED / 2;
  
  // Limit distance reading to the max size of the bin
  if (distanceCm > maxDistanceCm) {
    distanceCm = maxDistanceCm;
  }
  
  return distanceCm;
}

// Function to calculate fill percentage
int calculateFillPercentage(float dist) {
    // Reverse the distance for fill level
    float currentLevel = maxDistanceCm - dist; 
    
    // Calculate percentage based on total height
    int percent = map(currentLevel, 0, maxDistanceCm, 0, 100);
    
    // Ensure percentage is between 0 and 100
    if (percent < 0) return 0;
    if (percent > 100) return 100;
    return percent;
}

void setup() {
  Serial.begin(115200);
  
  // Set up the Ultrasonic Sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();
}

void loop() {
  // Check for an incoming client connection
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    
    // Read data from the client
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          // If the line is blank, two newline characters, it means the client's HTTP request has ended.
          if (currentLine.length() == 0) {
            
            // --- CORE LOGIC: MEASURE AND CALCULATE ---
            measureDistance();
            fillPercentage = calculateFillPercentage(distanceCm);
            
            // --- HTTP Response Header ---
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // --- HTML Web Page Content ---
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<title>Smart Dustbin Status</title>");
            client.println("<meta http-equiv='refresh' content='5'></head>"); // Refresh every 5 seconds
            client.println("<body>");
            
            client.println("<h1>IoT Smart Dustbin Status</h1>");
            
            client.print("<h2>Fill Level: ");
            client.print(fillPercentage);
            client.println("%</h2>");

            client.print("<p>Distance to Garbage: ");
            client.print(distanceCm);
            client.println(" cm</p>");
            
            client.println("<div style='width: 300px; height: 30px; border: 1px solid #ccc; background-color: #f0f0f0;'>");
            client.print("<div style='width: ");
            client.print(fillPercentage);
            client.print("%; height: 100%; background-color: ");
            // Set color based on fill level
            if (fillPercentage >= 80) {
              client.print("red");
              client.println("'></div></div>");
              client.println("<h3>STATUS: FULL - NEEDS EMPTYING</h3>");
            } else if (fillPercentage >= 50) {
              client.print("orange");
              client.println("'></div></div>");
              client.println("<h3>STATUS: HALF FULL</h3>");
            } else {
              client.print("green");
              client.println("'></div></div>");
              client.println("<h3>STATUS: EMPTY</h3>");
            }

            client.println("</body></html>");
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    
    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }
}