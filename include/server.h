#ifndef SERVER_H
#define SERVER_H

#include "serverCallbacks.h"
#include "serviceAuth.h"
#include "serviceRegister.h"
#include "services.h"
#include "fileSystem.h"
#include "lockstate.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLESecurity.h>

enum SERVICE
{
  AUTH,
  REGISTER
};

enum ACTION
{
  START,
  STOP
};

class BluetoothServer
{
private:
  BLEServer *pServer;
  BLEAdvertising *pAdvertising;
  ServiceCallbacks *pCallbacks;
  ServiceAuth *pServiceAuth;
  ServiceRegister *pServiceRegister;
  FileSystem *pFileSystem;

  void setup();
  void setupService();

public:
  BluetoothServer(std::string name, FileSystem *filesystem,  bool *shouldCheck, LOCKSTATE *state);
  ~BluetoothServer();
  void start();
  void manageService(SERVICE service, ACTION action);
  void checkState(bool registered);
};

#endif