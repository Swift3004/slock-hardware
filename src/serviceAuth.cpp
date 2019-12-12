#include "serviceAuth.h"

std::string ServiceAuth::getUUID() {
  return "ec01579e-4928-48ee-bed0-e68237efa95d";
}

void ServiceAuth::setupService() {
  pCharAuth1 = getService()->createCharacteristic(
    CHARACTERISTIC_UUID_AUTH_1,
    BLECharacteristic::PROPERTY_WRITE
  );
  // pCharAuth1->setCallbacks(new ServiceAuthCallbacks());
  pCharAuth1->addDescriptor(new BLE2902());
}

