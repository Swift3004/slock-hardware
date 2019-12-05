#ifndef SERVICEAUTH_H
#define SERVICEAUTH_H

#include "services.h"
#include "serviceCallbacks.h"

#include <BLE2902.h>

#define CHARACTERISTIC_UUID_AUTH_1 "ec01579e-4928-48ee-bed0-e68237efa95e"

class ServiceAuth: public Services {
  public:
    BLECharacteristic* pCharAuth1;
    void setup();
};
#endif