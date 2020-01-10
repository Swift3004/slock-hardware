#include "serviceCallbacks.h"

ServiceCallbacks::ServiceCallbacks(FileSystem *fileSystem, bool *shouldCheck, LOCKSTATE *state)
{
  pFileSystem = fileSystem;
  pShouldCheck = shouldCheck;
  pState = state;
}

void ServiceCallbacks::onWrite(BLECharacteristic *pCharacteristic)
{
  std::string rxValue = pCharacteristic->getValue();
  Serial.println("Received Something I Guess?");
  // Prevent Crash caused by Android!
  if (rxValue.length() <= 0)
  {
    return;
  }

  if (rxValue.length() > 0)
  {
    Serial.println("*********");
    Serial.print("Received Value: ");
    for (int i = 0; i < rxValue.length(); i++)
      Serial.print(rxValue[i]);

    Serial.println();
    Serial.println("*********");
  }

  Serial.println(pCharacteristic->getUUID().toString().c_str());
  if (pCharacteristic->getUUID().toString() == CHARACTERISTIC_UUID_REGISTER_1)
  {
    pFileSystem->writeFile(SD, "/name.txt", rxValue.c_str());
    *pShouldCheck = true;
  }
  else if (pCharacteristic->getUUID().toString() == CHARACTERISTIC_UUID_REGISTER_2)
  {
    pFileSystem->writeFile(SD, "/SECRET.txt", rxValue.c_str());
    pFileSystem->writeFile(SD, "/SECRET_2.txt", "0");
    *pShouldCheck = true;
  }
  else if (pCharacteristic->getUUID().toString() == CHARACTERISTIC_UUID_AUTH_1)
  {
    if (rxValue.length() < 89)
    {
      Serial.println("Don't do that, don't give me hope!");
      return;
    }

    std::string hash = rxValue.substr(0, 88);
    std::string action = rxValue.substr(89, rxValue.length() - 89);
    int actionInt = 0;
    std::string notification;

    if (action != "-1" && action != "0" && action != "1")
    {
      Serial.println("Don't do that, don't give me hope!");
      return;
    }

    actionInt = atoi(action.c_str());

    bool response = authenticate(hash.c_str());
    if (response)
    {
      Serial.println("I see this as an abosulte win!");
      *pState = static_cast<LOCKSTATE>(actionInt);
      notification = "200";
    }
    else
    {
      Serial.println(pFileSystem->readFile(SD, "/SECRET_2.txt").c_str());
      Serial.println("These are confusing times!");
      notification = "401;" + pFileSystem->readFile(SD, "/SECRET_2.txt");
    }
    char buffer[16];
    sprintf(buffer, notification.c_str());
    pCharacteristic->setValue((uint8_t *)buffer, strlen(buffer));
    pCharacteristic->notify();
  }
}

void ServiceCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
}

bool ServiceCallbacks::authenticate(const char *hash)
{
  bool response = false;

  std::string secret = pFileSystem->readFile(SD, "/SECRET.txt");
  std::string secret_2 = pFileSystem->readFile(SD, "/SECRET_2.txt");
  const char *noncalculatedHash = (secret + ";" + secret_2).c_str();
  std::string calculatedHash = "";

  SHA512 sha512;
  size_t size = strlen(noncalculatedHash);
  size_t posn, len;
  uint8_t value[64];

  sha512.reset();

  for (posn = 0; posn < size; posn += size)
  {
    len = size - posn;
    if (len > size)
      len = size;
    sha512.update(noncalculatedHash + posn, len);
  }
  sha512.finalize(value, sizeof(value));

  for (int count = 0; count < 64; count++)
  {
    char buffer[7];
    itoa(value[count], buffer, 16);
    if (std::string(buffer).length() == 1)
    {
      calculatedHash += "0";
    }
    calculatedHash += buffer;
  }

  if (strcmp(hash, calculatedHash.c_str()) == 0)
  {
    response = true;
    //pFileSystem->writeFile(SD, "/SECRET_2.txt", "" + atoi(secret_2.c_str()) + 1);
  }
  else
  {
    response = false;
  }

  return response;
}