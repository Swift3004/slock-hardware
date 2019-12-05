#ifndef SERVER_H
#define SERVER_H

#include "serverCallbacks.h"
#include "serviceAuth.h"
#include "services.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

enum SERVICE {
  AUTH
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
  void setup();
  void setupService();
  

public:
  BluetoothServer();
  ~BluetoothServer();
  void start();
  void manageService(SERVICE service, ACTION action);
};

#endif