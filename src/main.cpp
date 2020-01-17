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
bool run = false;
int clickCount = 0;
std::string savedClickCount;

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

void TaskLoop(void *parameter)
{
  for (;;)
  {
    char str[100];
    sprintf(str, "%d", clickCount);
    Serial.println(str);
    filesystem->writeFile(SD, "/position.txt", str);
    delay(1000);
  }
}

void setup()
{

  Serial.begin(115200);

  ESP32Encoder::useInternalWeakPullResistors = false;
  encoder.attachHalfQuad(21, 32);

  filesystem = new FileSystem();
  Serial.println(filesystem->readFile(SD, "/position.txt").c_str());
  savedClickCount = filesystem->readFile(SD, "/position.txt");
  if (savedClickCount.size() == 0)
  {
    Serial.println("Fuck");
    filesystem->writeFile(SD, "/position.txt", "0");
  }

  run = true;
  encoder.setCount(atoi(savedClickCount.c_str()));
  checkRegister();

  Serial.print("Registered is: ");
  Serial.println(registered);

  restart();

  xTaskCreatePinnedToCore(
      TaskLoop, /* Function to implement the task */
      "Task1",  /* Name of the task */
      10000,    /* Stack size in words */
      NULL,     /* Task input parameter */
      0,        /* Priority of the task */
      &Task1,   /* Task handle. */
      0);       /* Core where the task should run */
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
  while (!run)
  {
  }
  clickCount = encoder.getCount();

  switch (*state)
  {
  case IDLE:
    servo.detach();
    //Serial.println(clickCount);
    break;
  case OPEN:
    attachServo();
    if (clickCount >= 10)
    {
      servo.detach();
      *state = ZERO;
      delay(5000);
    }
    else
    {
      servo.write(1700);
      delay(20);
    }

    break;
  case CLOSE:
    attachServo();
    if (clickCount <= -39)
    {
      *state = IDLE;
    }
    else
    {
      // Serial.println(clickCount);
      servo.write(1300);
      delay(20);
    }
    break;
  case ZERO:
    attachServo();
    if (clickCount <= 1)
    {
      *state = IDLE;
    }
    else
    {
      servo.write(1300);
      delay(20);
    }
    break;
  default:
    break;
  }

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