#include "server.h"

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
  setupService();
}

void BluetoothServer::setupService()
{
  pServiceAuth->init(pServer);
  pServiceAuth->setupService();
  
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pServiceAuth->getUUID());
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this param

  BLEDevice::startAdvertising();
}

void BluetoothServer::manageService(SERVICE service, ACTION action)
{
  BLEService *serviceBLE = NULL;

  switch (service)
  {
  case AUTH:
    serviceBLE = pServiceAuth->getService();
    break;
  case REGISTER:
    serviceBLE = pServiceRegister->getService();
  default:
    break;
  }

  if (action == START)
  {
  serviceBLE->start();
  }
  else if (action == STOP)
  {
    serviceBLE->stop();
  }
}