#include "serviceRegister.h"

std::string ServiceRegister::getUUID() {
  return "7c3e0e35-996f-4745-a62f-ecb0d6e971b2";
}

void ServiceRegister::setupService(ServiceCallbacks* pCallbacks) {
  pCharRegister1 = getService()->createCharacteristic(
    CHARACTERISTIC_UUID_REGISTER_1,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharRegister1->setCallbacks(pCallbacks);
  pCharRegister1->addDescriptor(new BLE2902());
}