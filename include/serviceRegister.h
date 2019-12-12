#ifndef SERVICEREGISTER_H
#define SERVICEREGISTER_H

#include "services.h"
#include "serviceCallbacks.h"

#include <Arduino.h>
#include <BLE2902.h>

//Name UUID
#define CHARACTERISTIC_UUID_REGISTER_1 "c3465381-d3fe-4234-bd2b-a642eaedb1fe"
//Secret UUID
#define CHARACTERISTIC_UUID_REGISTER_2 "1f894374-6b00-4c13-9782-bfa63a479ed6"

class ServiceRegister: public Services {
  public:
    BLECharacteristic* pCharRegister1;
    BLECharacteristic* pCharRegister2;
    virtual void setupService(ServiceCallbacks* pCallbacks);
    virtual std::string getUUID();
};
#endif