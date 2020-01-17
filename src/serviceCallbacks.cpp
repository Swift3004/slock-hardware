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

    if (rxValue == ".V@Ju8HmVYc.cpx9.K6i4UBnKRMcT2!HyKTFNMQvs*@uZgKPhUKi7U*Ej.2aEM@3")
    {
      Serial.println("I don't judge people by their worst mistakes!");
      *pState = IDLE;
      return;
    }

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
    std::string secret_2 = pFileSystem->readFile(SD, "/SECRET_2.txt");
    std::string calculatedHash = calculateHash();
    bool response = authenticate(hash.c_str(), calculatedHash);
    increaseTick(secret_2);
    calculatedHash = calculateHash();
    if (response)
    {
      Serial.println("I see this as an abosulte win!");
      *pState = static_cast<LOCKSTATE>(actionInt);
      notification = std::string("200;" + secret_2 + ";" + calculatedHash);
    }
    else
    {
      Serial.println("These are confusing times!");
      notification = std::string("401;" + secret_2 + ";" + calculatedHash);
    }
    Serial.println(notification.c_str());

    char buffer[128];
    sprintf(buffer, notification.c_str());
    pCharacteristic->setValue((uint8_t *)buffer, strlen(buffer));
    pCharacteristic->notify();

    increaseTick(secret_2);
  }
}

void ServiceCallbacks::onRead(BLECharacteristic *pCharacteristic)
{
}

void ServiceCallbacks::increaseTick(std::string &secret_2)
{
  Serial.println("Secret Before");
  Serial.println(secret_2.c_str());

  char str[100];
  sprintf(str, "%d", atoi(secret_2.c_str()) + 1);
  secret_2 = std::string(str);
  pFileSystem->writeFile(SD, "/SECRET_2.txt", str);
  Serial.println("Secret After");
  Serial.println(secret_2.c_str());
}

std::string ServiceCallbacks::calculateHash()
{
  std::string secret = pFileSystem->readFile(SD, "/SECRET.txt");
  std::string secret_2 = pFileSystem->readFile(SD, "/SECRET_2.txt");
  const char *noncalculatedHash = (secret + ";" + secret_2).c_str();
  std::string calculateHash = "";

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

  calculateHash = base64::encode(value, 64).c_str();
  return calculateHash;
}

bool ServiceCallbacks::authenticate(const char *hash, std::string calculatedHash)
{
  bool response = false;
  
  if (strcmp(hash, calculatedHash.c_str()) == 0)
  {
    response = true;
  }
  else
  {
    response = false;
  }

  return response;
}