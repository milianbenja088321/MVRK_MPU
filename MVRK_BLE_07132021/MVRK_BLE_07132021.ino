
#define BOUNCE_WITH_PROMPT_DETECTION    // Make button state changes available immediately

#include "Wire.h"
#include <MPU6050_light.h>
#include <Bounce2.h>      // https://github.com/thomasfredericks/Bounce2
#include <BleGamepad.h>   // https://github.com/lemmingDev/ESP32-BLE-Gamepad

MPU6050 mpu(Wire);

#define numOfButtons 2

Bounce debouncers[numOfButtons];
BleGamepad bleGamepad("MVRKTEST", "Espressif", 100);

byte buttonPins[numOfButtons] = { 4, 12 };
byte physicalButtons[numOfButtons] = { 23, 2 };

bool didCalibrate = false;

void setup()
{
  Serial.begin(115200);
  Wire.begin();

Serial.println("Calibrating");

      byte status = mpu.begin();
      delay(1000);

      mpu.calcOffsets();
  
  
//  while (didCalibrate == false)
//  {
//    Serial.println("Needs Calibration");
//    debouncers[0].update();
//
//    if (debouncers[0].fell())
//    {
//      Serial.println("Calibrating");
//
//      byte status = mpu.begin();
//      delay(1000);
//
//      mpu.calcOffsets();
//      didCalibrate = true;
//    }
//  }

  for (byte currentPinIndex = 0; currentPinIndex < numOfButtons; currentPinIndex++)
  {
    pinMode(buttonPins[currentPinIndex], INPUT_PULLUP);

    debouncers[currentPinIndex] = Bounce();
    debouncers[currentPinIndex].attach(buttonPins[currentPinIndex]);      // After setting up the button, setup the Bounce instance :
    debouncers[currentPinIndex].interval(5);
  }

  bleGamepad.begin();
  Serial.println("Ready");

}

void loop()
{
  mpu.update();

  if (bleGamepad.isConnected())
  {


    for (byte currentIndex = 0; currentIndex < numOfButtons; currentIndex++)
    {
      debouncers[currentIndex].update();

      if (debouncers[currentIndex].fell())
      {
        bleGamepad.press(physicalButtons[currentIndex]);
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" pushed.");

        if(currentIndex = 0)
        {
          
        }
      
      }
      else if (debouncers[currentIndex].rose())
      {
        bleGamepad.release(physicalButtons[currentIndex]);
        Serial.print("Button ");
        Serial.print(physicalButtons[currentIndex]);
        Serial.println(" released.");
      }
    }


    float x = map(mpu.getAngleX(), -180, 180, 32737, -32737);
    float y = map(mpu.getAngleY(), -180, 180, 32737, -32737);

    bleGamepad.setX(x);
    delay(5);
    bleGamepad.setY(y);
    delay(5);

    Serial.print("Pitch: ");
    Serial.println(mpu.getAngleY());
    Serial.print("Yaw: ");
    Serial.println(mpu.getAngleZ());


    bleGamepad.sendReport();
    delay(20);  // (Un)comment to remove/add delay between loops
  }
}
