#include "serviceRegister.h"

std::string ServiceRegister::getUUID() {
  return "7c3e0e35-996f-4745-a62f-ecb0d6e971b2";
}

void ServiceRegister::setupService() {
  pCharRegister1 = getService()->createCharacteristic(
    CHARACTERISTIC_UUID_REGISTER_1,
    BLECharacteristic::PROPERTY_WRITE
  );
  // pCharAuth1->setCallbacks(new ServiceAuthCallbacks());
  pCharRegister1->addDescriptor(new BLE2902());
}