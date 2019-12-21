#ifndef SERVICEREGISTER_H
#define SERVICEREGISTER_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

class ServiceRegister: public Services {
  public:
    BLECharacteristic* pCharRegister1;
    BLECharacteristic* pCharRegister2;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif