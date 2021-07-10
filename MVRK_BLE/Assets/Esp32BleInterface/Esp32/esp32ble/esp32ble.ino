#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <ezButton.h>

#include "Wire.h"
#include <MPU6050_light.h>

// BLE ***************************
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

float x = 0.12;
float y = 0.13;
float z = 0.14;

float ypr[5]; // [yaw, pitch, roll, aState, bState]

// Button ***************************
ezButton triggerA(4);
ezButton triggerB(16);
bool aPressed = false;
bool bPressed = false;

// MPU ******************************
MPU6050 mpu(Wire);

// Timer ****************************
const long period = 100;
unsigned long previousMillis = 0;

class Esp32ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      value = 0;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};


void setup()
{
  Serial.begin(115200);
  
  triggerA.setDebounceTime(50);
  triggerB.setDebounceTime(50);

  Wire.begin();
  byte status = mpu.begin();
  // wait for calibration
  delay(2000);

  // mpu.upsideDownMounting = true; // Just in case the mounting is inverted
  mpu.calcOffsets(); // gyro and accelerometer

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
  pAdvertising->setScanResponse(true); // allow second device to connect
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

  CheckTriggers();

  // notify changed value
  if (deviceConnected)
  {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= period)
    {
      previousMillis = currentMillis;

      mpu.update();

      ypr[0] = mpu.getAngleX();
      ypr[1] = mpu.getAngleY();
      ypr[2] = mpu.getAngleZ();

      String temp = "";
      temp += ypr[0];
      temp += ",";
      temp += ypr[1];
      temp += ",";
      temp += ypr[2];
      temp += ",";
      temp += aPressed;
      temp += ",";
      temp += bPressed;

      //Serial.println(temp.c_str());
      
      pCharacteristic->setValue((char*)temp.c_str());
      pCharacteristic->notify();
    
    }
  }

  ////// only turn on for multi connectivity
//  // disconnecting
//  if (!deviceConnected && oldDeviceConnected) {
//    delay(500); // give the bluetooth stack the chance to get things ready
//    pServer->startAdvertising(); // restart advertising
//    Serial.println("start advertising");
//    oldDeviceConnected = deviceConnected;
//  }
//  // connecting
//  if (deviceConnected && !oldDeviceConnected) {
//    // do stuff here on connecting
//    oldDeviceConnected = deviceConnected;
//  }
}

void CheckTriggers()
{
  // class heartbeats must call first
  triggerA.loop();
  triggerB.loop();

  if (triggerA.isPressed())
  {
    Serial.println("A PRESSED");
    aPressed = true;
  }

  if (triggerB.isPressed())
  {
    Serial.println("B PRESSED");
    bPressed = true;
  }

  if (triggerA.isReleased())
  {
    //release action
    aPressed = false;
  }

  if (triggerB.isReleased())
  {
    // release action
    bPressed = false;
  }
}
