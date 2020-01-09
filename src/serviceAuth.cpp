#include "serviceAuth.h"

std::string ServiceAuth::getUUID()
{
  return SERVICE_UUID_AUTH;
}

void ServiceAuth::setupService(ServiceCallbacks *pCallbacks)
{
  pCharAuth1 = getService()->createCharacteristic(
      CHARACTERISTIC_UUID_AUTH_1,
      BLECharacteristic::PROPERTY_WRITE |
      BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharAuth1->setCallbacks(pCallbacks);
}
