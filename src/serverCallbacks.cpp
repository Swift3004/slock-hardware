#include "serverCallbacks.h"

ServerCallbacks::ServerCallbacks()
{
  connected = 0;
}

ServerCallbacks::~ServerCallbacks()
{
}

void ServerCallbacks::onConnect(BLEServer *server)
{
  connected++;
  Serial.println(connected);
  BLEDevice::startAdvertising();
}

void ServerCallbacks::onDisconnect(BLEServer *server)
{
  connected--;
  Serial.println(connected);
}