#ifndef SERVICECALLBACKS_H
#define SERVICECALLBACKS_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

class ServiceCallbacks : public BLECharacteristicCallbacks
{
public:
  void onWrite(BLECharacteristic *pCharacteristic);
  void onRead(BLECharacteristic *pCharacteristic);
};
#endif