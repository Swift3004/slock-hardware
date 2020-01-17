#ifndef SERVICEREGISTER_H
#define SERVICEREGISTER_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

#define SERVICE_UUID_REGISTER "7c3e0e35-996f-4745-a62f-ecb0d6e971b2"

class ServiceRegister: public Services {
  public:
    BLECharacteristic* pCharRegister1;
    BLECharacteristic* pCharRegister2;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif