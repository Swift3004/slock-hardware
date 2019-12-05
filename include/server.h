#ifndef SERVER_H
#define SERVER_H

#include "serverCallbacks.h"
#include "serviceAuth.h"
#include "services.h"

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
  ServiceAuth *pServiceAuth; 
  void setup();
  void setupService();
  void manageService(SERVICE service, ACTION action);

public:
  BluetoothServer();
  ~BluetoothServer();
  void start();
};

#endif