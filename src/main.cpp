#include "server.h"

#include <Arduino.h>

BluetoothServer* server = NULL;

void setup() {
  Serial.begin(115200);
  server = new BluetoothServer();
  server->start();
  Serial.println("Test");
  server->manageService(AUTH, START);
}

void loop() {
  
}