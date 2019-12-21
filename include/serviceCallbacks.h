#ifndef SERVICECALLBACKS_H
#define SERVICECALLBACKS_H

#include "fileSystem.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <Crypto.h>
#include <SHA512.h>
#include <string.h>

// Token UUID
#define CHARACTERISTIC_UUID_AUTH_1 "b2d709fe-de1c-46db-9b13-a39aac60de42"

//Name UUID
#define CHARACTERISTIC_UUID_REGISTER_1 "c3465381-d3fe-4234-bd2b-a642eaedb1fe"
//Secret UUID
#define CHARACTERISTIC_UUID_REGISTER_2 "1f894374-6b00-4c13-9782-bfa63a479ed6"

class ServiceCallbacks : public BLECharacteristicCallbacks
{
private:
  FileSystem *pFileSystem;
  bool *pShouldCheck;
public:
  ServiceCallbacks(FileSystem *fileSystem, bool *shouldCheck);
  ~ServiceCallbacks();
  void onWrite(BLECharacteristic *pCharacteristic);
  void onRead(BLECharacteristic *pCharacteristic);
  bool authenticate(const char* hash);
};
#endif 