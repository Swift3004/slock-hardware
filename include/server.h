#ifndef SERVER_H
#define SERVER_H

#include "serverCallbacks.h"
#include "serviceAuth.h"
#include "serviceRegister.h"
#include "services.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

enum SERVICE {
  AUTH,
  REGISTER
};

enum ACTION {
  START,
  STOP
};

class BluetoothServer
{
private:
  BLEServer* pServer;
  BLEAdvertising* pAdvertising;
  ServiceAuth *pServiceAuth; 
  ServiceRegister *pServiceRegister; 
  void setup();
  void setupService();
  

public:
  BluetoothServer();
  ~BluetoothServer();
  void start();
  void manageService(SERVICE service, ACTION action);
  void checkState(bool registered);
};

#endif