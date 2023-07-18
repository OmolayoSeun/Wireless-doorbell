#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t broadcastAddress[] = { 0xFC, 0xF5, 0xC4, 0x96, 0xEC, 0x2B };


typedef struct struct_message {
  bool state;
} struct_message;

struct_message data;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  } else {
    Serial.println("Delivery fail");
  }
}


void setup() {
  
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // rings bell from rest
  rst_info *resetInfo = ESP.getResetInfoPtr();

  if (resetInfo->reason == REASON_DEEP_SLEEP_AWAKE) {
    data.state = true;
    esp_now_send(broadcastAddress, (uint8_t *)&data, sizeof(data));
    Serial.println("\nrings bell");
  }

  delay(100);
  Serial.println("Esp going to sleep...");
  ESP.deepSleep(0);
}

void loop() {}