#include "main.h"

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer* pServerMain = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEAdvertising *pAdvertising = NULL;
 BLEService *pService2 = NULL;
uint32_t deviceConnected = 0;
uint32_t oldDeviceConnected = 0;
uint32_t value = 0;
uint32_t value2 = 0;
uint32_t value3 = 0;
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public BLEServerCallbacks {

    void onConnect(BLEServer* pServer) {
      deviceConnected++;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected--;
      value3 = 1;
//BLEDevice::startAdvertising();
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

       Serial.println();
      Serial.println("*********");
    }
  }

  void onRead(BLECharacteristic *pCharacteristic) {
     Serial.print("aaaReceived Value: ");
  }
};


void setup() {
  Serial.begin(115200);

  // Create the BLE Device
  BLEDevice::init("ESP32");

  // Create the BLE Server
  pServerMain = BLEDevice::createServer();
  pServerMain->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServerMain->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
  pCharacteristic->setCallbacks(new MyCallbacks());
  
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter



  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
    // notify changed value
    if (deviceConnected > 0) {
        //Serial.println(deviceConnected);
        pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->notify();
        value++;
        delay(10); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!(deviceConnected > 0) && oldDeviceConnected > 0) {
       Serial.println("3");
        delay(500); // give the bluetooth stack the chance to get things ready
        pServerMain->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected > 0 && !(oldDeviceConnected > 0)) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }

    if (value2 == 0 && deviceConnected > 0) {
      Serial.println("yo");
       pService2 = pServerMain->createService("4fafc201-1fb5-459e-8fcc-c5c9c331915c");

  // Create a BLE Characteristic
  BLECharacteristic* pCharacteristic2 = pService2->createCharacteristic(
                      "beb5483e-36e1-4688-b7f5-ea07361b26b9",
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
 
  
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic2->addDescriptor(new BLE2902());

  // Start the service
 // pService2->start();
  BLEDevice::getAdvertising()->addServiceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331915c");
  BLEDevice::startAdvertising();
  value2 = 1;
    }

    if (value3 == 1) {
      pService2->start();
      value3 = 10;
    }
}