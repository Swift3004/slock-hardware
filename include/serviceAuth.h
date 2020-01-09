#ifndef SERVICEAUTH_H
#define SERVICEAUTH_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

#define SERVICE_UUID_AUTH "ec01579e-4928-48ee-bed0-e68237efa95d"

class ServiceAuth: public Services {
  public:
    BLECharacteristic* pCharAuth1;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif