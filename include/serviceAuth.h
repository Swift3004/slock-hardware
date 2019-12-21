#ifndef SERVICEAUTH_H
#define SERVICEAUTH_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>


class ServiceAuth: public Services {
  public:
    BLECharacteristic* pCharAuth1;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif