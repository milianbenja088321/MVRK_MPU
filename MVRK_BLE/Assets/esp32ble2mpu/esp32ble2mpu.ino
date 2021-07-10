
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <ezButton.h>

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
MPU6050 MPU2(Wire);

unsigned long timer = 0;
unsigned long period = 250;

float ypr[8];

// BLE **************************
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// Button ***************************
ezButton triggerA(4);
ezButton triggerB(16);

bool aPressed = false;
bool bPressed = false;


class Esp32ServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      value = 0;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void InitBLE()
{
  // Create the BLE Device
  BLEDevice::init("Esp32Ble");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new Esp32ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
}

void InitButtons()
{
  triggerA.setDebounceTime(50);
  triggerB.setDebounceTime(50);
}

void CheckTriggers()
{
  // class heartbeats must call first
  triggerA.loop();
  triggerB.loop();

  if (triggerA.isPressed())
  {
    aPressed = true;
    Serial.println("A");
  }

  if (triggerB.isPressed())
  {
    bPressed = true;
    Serial.println("B");
  }

  if (triggerA.isReleased())
  {
    aPressed = false;
  }

  if (triggerB.isReleased())
  {
    bPressed = false;
  }
}


void setup()
{
  Serial.begin(115200);
  Wire.begin();

  MPU2.setAddress(0x69);

  byte status = mpu.begin();
  Serial.print(F("MPU1 status: "));
  Serial.println(status);
  //while (status != 0) { } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);

  mpu.calcOffsets();

  status = MPU2.begin();
  Serial.print(F("MPU2 status: "));
  Serial.println(status);
  //while (status != 0) { } // stop everything if could not connect to MPU6050

  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);

  // mpu.upsideDownMounting = true; // uncomment this line if the MPU6050 is mounted upside-down
  MPU2.calcOffsets(); // gyro and accelero
  
  InitButtons();
  InitBLE();

  
  Serial.println("Waiting a client connection to notify...");
}

void loop()
{

  // notify changed value
  if (deviceConnected)
  {
    CheckTriggers();
    if (millis() - timer > period)
    {
      mpu.update();
      MPU2.update();
      
      timer = millis();
      ypr[0] = mpu.getAngleX();
      ypr[1] = mpu.getAngleY();
      ypr[2] = mpu.getAngleZ();
      ypr[3] = MPU2.getAngleX();
      ypr[4] = MPU2.getAngleY();
      ypr[5] = MPU2.getAngleZ();
      ypr[6] = aPressed;
      ypr[7] = bPressed;

      // have values stored in advanced for buffer into string
      String temp = "";
      temp += ypr[0];
      temp += ",";
      temp += ypr[1];
      temp += ",";
      temp += ypr[2];
      temp += ",";
      temp += ypr[3];
      temp += ",";
      temp += ypr[4];
      temp += ",";
      temp += ypr[5];
      temp += ",";
      temp += ypr[6];
      temp += ",";
      temp += ypr[7];

      Serial.println(temp.c_str());
      
      pCharacteristic->setValue((char*)temp.c_str());
      pCharacteristic->notify();

    }
    //    // disconnecting
    //    if (!deviceConnected && oldDeviceConnected)
    //    {
    //      delay(500); // give the bluetooth stack the chance to get things ready
    //      pServer->startAdvertising(); // restart advertising
    //      Serial.println("start advertising");
    //      oldDeviceConnected = deviceConnected;
    //    }
    //    // connecting
    //    if (deviceConnected && !oldDeviceConnected)
    //    {
    //      // do stuff here on connecting
    //      Serial.println("Connecting");
    //      oldDeviceConnected = deviceConnected;
    //    }
  }
}
