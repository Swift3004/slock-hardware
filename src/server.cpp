#include "server.h"
#include "serviceAuth.h"

BluetoothServer::BluetoothServer()
{
  BLEDevice::init("SLOCK-ALPHA-v1");
  pServer = BLEDevice::createServer();
  pServiceAuth = new ServiceAuth();
}

BluetoothServer::~BluetoothServer()
{
}

void BluetoothServer::start()
{
  setup();
}

void BluetoothServer::setup()
{
  pServer->setCallbacks(new ServerCallbacks());
}

void BluetoothServer::setupService()
{
    pServiceAuth->init(pServer);
    pServiceAuth->setup();
   
}

void BluetoothServer::manageService(SERVICE service, ACTION action) {
  BLEService* serviceBLE = NULL;
  
  switch (service)
  {
  case AUTH:
    serviceBLE = pServiceAuth->pService;
    break;
  default:
    break;
  }

if (action == START) {
  serviceBLE->start();
}else if (action == STOP) {
  serviceBLE->stop();
}

}