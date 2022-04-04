#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

// Pin configuration
#define LED_PIN 16
#define DHT_PIN 2

// General configuration
#define API_KEY "test"
#define SERVER "https://eny2j6loaimbm.x.pipedream.net"
#define DELAY_MS 5000
#define SSID "iPhone"
#define PASSWORD "ciaocomestai"


DHT dht(DHT_PIN, DHT11);

void setup () {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(SSID, PASSWORD);

  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);
    delay(500);
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, ip address: ");
  Serial.println(WiFi.localIP());
  
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  dht.begin();
  Serial.println("DHT Initialized!");
}

void sendData(String name, float value, String unit) {

  WiFiClientSecure client;
  client.setInsecure();
  client.connect(SERVER, 443);
  
  HTTPClient http;

  http.begin(client, SERVER);

  http.addHeader("X-API-Key", API_KEY);
  http.addHeader("Content-Type", "application/json");

  Serial.println("Sending sensors data...");
  String data = "{\"name\": \"" + name + "\",\"value\": " + value + ",\"unit\": \"" + unit + "\"}";
  int httpCode = http.POST(data);
  Serial.printf("Status code: %d\n", httpCode);
  String payload = http.getString();
  Serial.println(payload);

  http.end();

}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_PIN, LOW);

    Serial.println("Reading sensor data");
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      Serial.printf("Humidity: %f\n", humidity);
      Serial.printf("Temperature: %f\n", temperature);

      sendData("temperature", temperature, "c");
      sendData("humidity", humidity, "h");

    } else {
      Serial.println("Error, could not read the sensors");
    }
  } else {
    Serial.println("Error, no WiFi connection!");
    digitalWrite(LED_PIN, HIGH);
  }
  delay(DELAY_MS);
}
