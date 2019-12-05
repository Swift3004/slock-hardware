#include "serviceAuth.h"

std::string Services::getUUID() {
  return "7c3e0e35-996f-4745-a62f-ecb0d6e971b2";
}

void ServiceAuth::setup() {
  pCharAuth1 = pService->createCharacteristic(
    CHARACTERISTIC_UUID_AUTH_1,
    BLECharacteristic::PROPERTY_WRITE
  );
  // pCharAuth1->setCallbacks(new ServiceAuthCallbacks());
  pCharAuth1->addDescriptor(new BLE2902());
}

