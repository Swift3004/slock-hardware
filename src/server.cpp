#include "server.h"
#include "serverCallbacks.h"

BluetoothServer::BluetoothServer()
{
  BLEDevice::init("SLOCK-ALPHA-v1");
  pServer = BLEDevice::createServer();
  pCallbacks = new ServiceCallbacks();
  pServiceAuth = new ServiceAuth();
  pServiceRegister = new ServiceRegister();
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
  pServiceAuth->setupService(pCallbacks);

  pServiceRegister->init(pServer);
  pServiceRegister->setupService(pCallbacks);

  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(pServiceAuth->getUUID());
  pAdvertising->addServiceUUID(pServiceRegister->getUUID());
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
    break;
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

void BluetoothServer::checkState(bool registered)
{
  if (registered == false)
  {
    manageService(REGISTER, START);
    manageService(AUTH, STOP);
  }
  else
  {
    manageService(REGISTER, STOP);
    manageService(AUTH, START);
  }
}

void ServiceCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string rxValue = pCharacteristic->getValue();

  if (rxValue.length() > 0)
  {
    Serial.println("*********");
    Serial.print("Received Value: ");
    for (int i = 0; i < rxValue.length(); i++)
      Serial.print(rxValue[i]);

    Serial.println();
    Serial.println("*********");
  }
}

void ServiceCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
}