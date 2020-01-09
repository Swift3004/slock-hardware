#include "serviceRegister.h"

std::string ServiceRegister::getUUID() {
  return SERVICE_UUID_REGISTER;
}

void ServiceRegister::setupService(ServiceCallbacks* pCallbacks) {
  pCharRegister1 = getService()->createCharacteristic(
    CHARACTERISTIC_UUID_REGISTER_1,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharRegister1->setCallbacks(pCallbacks);

  pCharRegister2 = getService()->createCharacteristic(
    CHARACTERISTIC_UUID_REGISTER_2,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharRegister2->setCallbacks(pCallbacks);
}