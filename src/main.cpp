#include "server.h"

#include <Arduino.h>
#include <ESP32Encoder.h>
#include <ESP32Servo.h>

#define ROTARY_ENCODER_PIN1 21
#define ROTARY_ENCODER_PIN2 32
#define SERVO_PIN 13

BluetoothServer *server = NULL;

FileSystem *filesystem;

std::string name;
std::string secret;
bool registered = false;
bool *shouldCheck = new bool(false);

LOCKSTATE *state = new LOCKSTATE(IDLE);

ESP32Encoder encoder;

TaskHandle_t Task1;

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
  }
  else
  {
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
  server = new BluetoothServer(name, filesystem, shouldCheck, state);

  Serial.println("Starting BLE Server!");

  server->start();

  Serial.println("Starting Service!");

  server->checkState(registered);
}
uint32_t value = 0;
void TaskLoop(void* parameter) {
  for(;;) {
    //  char buf[100];
    //     sprintf(buf, "This is a long string to test if that will lead to memory loss %d", value);
    //     server->pServiceAuth->pCharAuth1->setValue((uint8_t*)buf, strlen(buf));
    //     server->pServiceAuth->pCharAuth1->notify();
    //     Serial.print("Freeheap: ");
    //     Serial.println(ESP.getFreeHeap());
    //     value++;
    delay(5000);
  }
}

void setup()
{

  Serial.begin(115200);

  ESP32Encoder::useInternalWeakPullResistors = false;
  encoder.attachHalfQuad(21, 32);

  filesystem = new FileSystem();
  filesystem->writeFile(SD, "/name.txt", "");
  filesystem->writeFile(SD, "/SECRET.txt", "");
  filesystem->writeFile(SD, "/SECRET_2.txt", "");
  checkRegister();

  Serial.print("Registered is: ");
  Serial.println(registered);

  restart();

  // xTaskCreatePinnedToCore(
  //     TaskLoop, /* Function to implement the task */
  //     "Task1",   /* Name of the task */
  //     10000,     /* Stack size in words */
  //     NULL,      /* Task input parameter */
  //     0,         /* Priority of the task */
  //     &Task1,    /* Task handle. */
  //     0);        /* Core where the task should run */
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