#ifndef SERVICES_H
#define SERVICES_H

#include <BLEServer.h>

class Services
{
private:
public:
  BLEService *pService;
  virtual void init(BLEServer *pServer);
  std::string getUUID();
  virtual void setup() = 0;
};
#endif