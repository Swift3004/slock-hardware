#ifndef SERVICEAUTH_H
#define SERVICEAUTH_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

#define CHARACTERISTIC_UUID_AUTH_1 "b2d709fe-de1c-46db-9b13-a39aac60de42"

class ServiceAuth: public Services {
  public:
    BLECharacteristic* pCharAuth1;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif