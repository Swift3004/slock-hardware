#include "services.h"

void Services::init(BLEServer* pServer) {
  pService = pServer->createService(getUUID());
}

