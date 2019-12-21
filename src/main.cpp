#include "server.h"

#include <Arduino.h>

BluetoothServer *server = NULL;

FileSystem *filesystem;

std::string name;
std::string secret;
bool registered = false;
bool *shouldCheck = new bool(false);

bool checkRegister()
{
  bool refresh = registered;
  name = "";
  secret = "";
  Serial.println("TEst");
  name = filesystem->readFile(SD, "/name.txt");
  secret = filesystem->readFile(SD, "/SECRET.txt");
  Serial.println(name.c_str());
  if (!(name.size() > 0) || !(secret.size() > 0))
  {
    Serial.println("TEst2");
    name = "SLOCK-ALPHA-v1";
    registered = false;
    // esp_efuse_mac_get_default();
  }else {
    registered = true;
  }

  if (refresh != registered)
  {
    refresh = true;
  }

  return refresh;
}

void restart()
{
  server = new BluetoothServer(name, filesystem, shouldCheck);

  Serial.println("Starting BLE Server!");

  server->start();

  Serial.println("Starting Service!");

  server->checkState(registered);
}

void setup()
{

  Serial.begin(115200);
  filesystem = new FileSystem();

  checkRegister();

  Serial.print("Registered is: ");
  Serial.println(registered);

  restart();
}

void loop()
{
  if (*shouldCheck == true)
  {
    Serial.println("ShouldCheck is true");
    if (checkRegister())
    {
      Serial.println("CheckRegister is true");
      server->~BluetoothServer();
      restart();
    }
    *shouldCheck = false;
  }
}