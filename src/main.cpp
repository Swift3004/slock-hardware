#include "server.h"

#include <Arduino.h>

BluetoothServer* server = NULL;

void setup() {
  Serial.begin(115200);
  server = new BluetoothServer();
  Serial.println("Starting BLE Server!");
  server->start();
  Serial.println("Starting AUTH Service!");
  server->manageService(AUTH, START);
}

void loop() {
  
}