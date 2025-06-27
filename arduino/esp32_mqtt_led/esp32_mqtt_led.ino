#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Family";
const char* password = "1234567890ok";

const char* mqtt_server = "broker.emqx.io";
WiFiClient espClient;
PubSubClient client(espClient);

#define LED_PIN 2

void sendStatus(bool isOn) {
  const char* statusMsg = isOn ? "on" : "off";
  client.publish("esp32/led/status", statusMsg);
}

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }

  Serial.println("Message received: " + msg);
  if (msg == "on") {
    digitalWrite(LED_PIN, HIGH);
    sendStatus(true);
  } else if (msg == "off") {
    digitalWrite(LED_PIN, LOW);
    sendStatus(false);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe("esp32/led");
      sendStatus(digitalRead(LED_PIN) == HIGH);  // ส่งสถานะเริ่มต้น
    } else {
      delay(2000);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}