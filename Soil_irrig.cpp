#include <DHT.h> 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define DHT_PIN D2
#define DHT_TYPE DHT11
#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN D3

char auth[] = "ol55yCkc7YHWJofo1RX0pNi1Skfi_mnB"; // Blynk authentication token
char ssid[] = "My Wifi"; //wifi name
char pass[] = "password"; // wifi password


DHT dht(DHT_PIN, DHT_TYPE);

BlynkTimer timer;

void sendSensorData() {
  float temperature = dht.readTemperature();  // Read temperature in Celsius
  float humidity = dht.readHumidity();        // Read humidity
  
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);
  int moisturePercentage = map(soilMoisture, 0, 1023, 0, 100);
  
  Blynk.virtualWrite(V1, temperature); // Send temperature data to virtual pin V0
  Blynk.virtualWrite(V2, humidity);    // Send humidity data to virtual pin V1
  Blynk.virtualWrite(V4, moisturePercentage); // Send soil moisture data to virtual pin V2
  
  // Control the relay based on soil moisture value
  if (moisturePercentage >= 100) {
    digitalWrite(RELAY_PIN, LOW); // Turn on relay
  } else if (moisturePercentage < 50) {
    digitalWrite(RELAY_PIN, HIGH); // Turn off relay
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as output
  
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(2000L, sendSensorData); // Send sensor data every 2 seconds
}

void loop() {
  Blynk.run();
  timer.run();
}
