#include "server.h"
#include "serverCallbacks.h"

BluetoothServer::BluetoothServer(std::string name, FileSystem *filesystem, bool *shouldCheck, LOCKSTATE *state)
{
  BLEDevice::init(name);
  pServer = BLEDevice::createServer();
  pFileSystem = filesystem;
  pServiceRegister = new ServiceRegister();
  pServiceAuth = new ServiceAuth();
  pCallbacks = new ServiceCallbacks(pFileSystem, shouldCheck, state);
}

BluetoothServer::~BluetoothServer()
{
  BLEDevice::deinit();
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

  // pServiceAuth->getService()->start();
  // pServiceRegister->getService()->start();

  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID_AUTH);
  pAdvertising->addServiceUUID(SERVICE_UUID_REGISTER);
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
    if (action == START)
    {
      pServiceAuth->init(pServer);
      pServiceAuth->setupService(pCallbacks);
    }
    serviceBLE = pServiceAuth->getService();
    break;
  case REGISTER:
    if (action == START)
    {
      pServiceRegister->init(pServer);
      pServiceRegister->setupService(pCallbacks);
    }
    serviceBLE = pServiceRegister->getService();
    break;
  default:
    break;
  }

  if (action == START && serviceBLE != nullptr)
  {
    serviceBLE->start();
  }
  else if (action == STOP && serviceBLE != nullptr)
  {
    pServer->removeService(serviceBLE);
  }
  
}

void BluetoothServer::checkState(bool registered)
{
  if (registered == false)
  {
    manageService(REGISTER, START);
    manageService(AUTH, STOP);
  }
  else
  {
    manageService(AUTH, START);
    manageService(REGISTER, STOP);
  }
}
