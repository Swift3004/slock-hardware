#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>

class ServerCallbacks: public BLEServerCallbacks
{
private:
  uint32_t connected;
public:
  ServerCallbacks();
  ~ServerCallbacks();
  void onConnect(BLEServer *server);
  void onDisconnect(BLEServer *server);
};