#include <ESP8266WiFi.h>
#include <espnow.h>

#define trigger 2

typedef struct struct_message {
  bool state;
} struct_message;

struct_message data;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t *mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&data, incomingData, sizeof(data));
  
  if (data.state == 1) {
    digitalWrite(trigger, HIGH);
    Serial.println("Received");
    delay(200);

    digitalWrite(trigger, LOW);
  }
}

void setup() {

  pinMode(trigger, OUTPUT);
  
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }


  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  
}

void loop() {
}