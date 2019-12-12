#ifndef SERVICES_H
#define SERVICES_H

#include <Arduino.h>
#include <BLEServer.h>

class Services
{
protected:
  BLEService *pService;
private:
public:
  virtual void init(BLEServer *pServer);
  virtual std::string getUUID() = 0;
  virtual void setupService() = 0;
  BLEService* getService() {return pService;}
  void setService(BLEService* service) {pService = service;}
};
#endif