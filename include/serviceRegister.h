#ifndef SERVICEREGISTER_H
#define SERVICEREGISTER_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

#define CHARACTERISTIC_UUID_REGISTER_1 "c3465381-d3fe-4234-bd2b-a642eaedb1fe"

class ServiceRegister: public Services {
  public:
    BLECharacteristic* pCharRegister1;
    virtual void setupService();
    virtual std::string getUUID();
};
#endif