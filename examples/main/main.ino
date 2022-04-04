#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "DHT.h"

// Pin configuration
#define DHT_PIN 2

// General configuration
#define NAME "in-"
#define API_KEY "test"
#define SERVER "https://en9x7spkrqyob.x.pipedream.net"
#define DELAY_MS 5000
#define SSID "iPhone"
#define PASSWORD "ciaocomestai"

DHT dht(DHT_PIN, DHT11);

void setup () {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.begin(SSID, PASSWORD);

  Serial.println();
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(250);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);
    }
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

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.println("Reading sensor data");
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      Serial.printf("Humidity: %f\n", humidity);
      Serial.printf("Temperature: %f\n", temperature);

      WiFiClientSecure client;
      client.setInsecure();
      client.connect(SERVER, 443);

      HTTPClient http;

      http.begin(client, SERVER);

      http.addHeader("X-API-Key", API_KEY);
      http.addHeader("Content-Type", "application/json");

      Serial.println("Sending sensors data...");
      String data = "[{\"name\": \"" + String(NAME) + "temperature\",\"value\": " + temperature + ",\"unit\": \"c\"},{\"name\": \"" + String(NAME) + "humidity\",\"value\": " + humidity + ",\"unit\": \"h\"}]";
      int httpCode = http.POST(data);
      Serial.printf("Status code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);

      http.end();

    } else {
      Serial.println("Error, could not read the sensors");
      digitalWrite(LED_BUILTIN, LOW);
    }
  } else {
    Serial.println("Error, no WiFi connection!");
    digitalWrite(LED_BUILTIN, LOW);
  }
  delay(DELAY_MS);
}
