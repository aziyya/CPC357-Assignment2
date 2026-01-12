#include <WiFi.h>
#include <PubSubClient.h>

// --- CONFIGURATION ---
const char* WIFI_SSID     = "WIFI_SSID";
const char* WIFI_PASSWORD = "WIFI_PASSWORD";
const char* MQTT_SERVER   = "EXTERNAL_IP_INSTANCE"; // Member 2 IP
const int   MQTT_PORT     = MQTT_PORT;
const char* MQTT_TOPIC    = "MQTT_TOPIC";

const int flamePin = 21; // Digital Pin D0

WiFiClient espClient;
PubSubClient client(espClient);
char buffer[256];

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // If you haven't set a password yet, use this:
    if (client.connect("ESP32_Flame_Node")) { 
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(flamePin, INPUT);
  
  setup_wifi();
  client.setServer(MQTT_SERVER, MQTT_PORT);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // --- READ SENSOR ---
  // Flame Sensor Logic: LOW (0) means Fire, HIGH (1) means Safe
  int flameState = digitalRead(flamePin);

  // --- DEBUGGING ---
  if (flameState == LOW) {
    Serial.println("!!! FIRE DETECTED !!!");
  } else {
    Serial.println("Status: Safe");
  }

  // --- SEND TO CLOUD ---
  // JSON: {"fire": true, "message": "EVACUATE"}
  const char* msg = (flameState == LOW) ? "EVACUATE" : "SAFE";
  const char* fireBool = (flameState == LOW) ? "true" : "false";

  sprintf(buffer, "{\"fire\": %s, \"message\": \"%s\"}", fireBool, msg);
  
  client.publish(MQTT_TOPIC, buffer);

  delay(2000); // Send update every 2 seconds
}