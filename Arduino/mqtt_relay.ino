#include <WiFi.h>
#include <PubSubClient.h>

// ─── WiFi Credentials ───────────────────────────────────────────
const char* ssid        = "--";
const char* password    = "--";

// ─── MQTT Broker ────────────────────────────────────────────────
const char* mqttServer   = "192.168.160.8";
const int   mqttPort     = 1883;
const char* mqttUser     = "esp32screen";
const char* mqttPassword = "123";
const char* mqttClientID = "ESP32_RELAY";
const char* mqttTopic    = "home/esp32/relay";

// ─── Hardware ───────────────────────────────────────────────────
#define RELAY_PIN 16
#define LED_PIN 23

// ─── Clients ────────────────────────────────────────────────────
WiFiClient   espClient;
PubSubClient client(espClient);

// ────────────────────────────────────────────────────────────────
void connectWiFi() {
  Serial.print("[WiFi] Connecting to: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("[WiFi] Connected!");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
}

// ────────────────────────────────────────────────────────────────
void connectMQTT() {
  while (!client.connected()) {
    Serial.print("[MQTT] Attempting connection... ");

    bool connected = (strlen(mqttUser) > 0)
      ? client.connect(mqttClientID, mqttUser, mqttPassword)
      : client.connect(mqttClientID);

    if (connected) {
      Serial.println("Connected!");
      client.subscribe(mqttTopic);
      Serial.print("[MQTT] Subscribed to: ");
      Serial.println(mqttTopic);
      digitalWrite(LED_PIN, HIGH);
    } else {
      Serial.print("Failed. State: ");
      Serial.print(client.state());
      Serial.println(" — retrying in 5s...");
      digitalWrite(LED_PIN, LOW);
      delay(5000);
    }
  }
}

// ────────────────────────────────────────────────────────────────
void callback(char* topic, byte* msg, unsigned int len) {
  String payload(msg, len);

  Serial.print("[MQTT] Payload: ");
  Serial.println(payload);

  if (payload == "ON") {
    digitalWrite(RELAY_PIN, LOW);   // LOW = ON for most relay modules
    Serial.println("[RELAY] ON");
  } else if (payload == "OFF") {
    digitalWrite(RELAY_PIN, HIGH);  // HIGH = OFF
    Serial.println("[RELAY] OFF");
  } else {
    Serial.println("[RELAY] Unknown command — ignored");
  }
}

// ────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // start OFF
  Serial.println("[RELAY] Initialized OFF");

  connectWiFi();

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  connectMQTT();

  Serial.println("[BOOT] Ready. Waiting for MQTT messages...");
}

// ────────────────────────────────────────────────────────────────
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[WiFi] Lost connection, reconnecting...");
    connectWiFi();
  }

  if (!client.connected()) {
    Serial.println("[MQTT] Lost connection, reconnecting...");
    connectMQTT();
  }

  client.loop();
}