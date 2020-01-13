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
Servo servo;

TaskHandle_t Task1;

int clickCount = 0;

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
void TaskLoop(void *parameter)
{
  for (;;)
  {
    delay(5000);
  }
}

void setup()
{

  Serial.begin(115200);

  ESP32Encoder::useInternalWeakPullResistors = false;
  encoder.attachHalfQuad(21, 32);

  filesystem = new FileSystem();

  if (filesystem->readFile(SD, "/position.txt") == "")
  {
    filesystem->writeFile(SD, "/position.txt", "0");
  }

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

void attachServo()
{
  if (!servo.attached())
  {
    servo.attach(SERVO_PIN);
  }
}

void loop()
{
  int savedClickCount = atoi(filesystem->readFile(SD, "/position.txt").c_str());
  if (encoder.getCount() != savedClickCount)
  {
    clickCount = encoder.getCount() + savedClickCount;
  } else {
    clickCount = encoder.getCount();
  }
  switch (*state)
  {
  case IDLE:
    servo.detach();
    break;
  case OPEN:
    attachServo();
    if (clickCount >= 5)
    {
      *state = IDLE;
    }
    else
    {
      servo.write(1700);
    }

    break;
  case CLOSE:
    attachServo();
    if (clickCount <= -40)
    {
      *state = IDLE;
    }
    else
    {
      servo.write(1300);
    }
    break;
  default:
    break;
  }
  filesystem->writeFile(SD, "/position.txt", "" + clickCount);
  delay(20);
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