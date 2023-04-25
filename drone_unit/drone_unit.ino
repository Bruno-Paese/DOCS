#include "MPU9250.h"
#include <SPI.h>
#include <Wire.h>
#include "RF24.h"
#include "printf.h"

#define DEBUG false

//5 Bottom
//6 Front
RF24 SendRadio(9, 10);
MPU9250 mpu;



void setup() {
  bool serialOpenFlag = true;
  // put your setup code here, to run once:
  Serial.begin(9600);
  printf_begin();
  Wire.begin();

  if (!mpu.setup(0x68)) {  // change to your own address
      while (1) {
          delay(5000);
      }
  }

  SendRadio.begin();
  SendRadio.setAddressWidth(5);
  SendRadio.openWritingPipe(0xF0F0FFF066LL);
  SendRadio.setChannel(115);           //115 band above WIFI signals
  SendRadio.setPALevel(RF24_PA_MAX);   //MIN power low rage
  SendRadio.setDataRate(RF24_1MBPS) ;  //Minimum speed
  SendRadio.stopListening(); //Stop Receiving and start transminitng
  SendRadio.printDetails();
  delay(500);


}

void WriteData(float value)
{
  SendRadio.openWritingPipe(0xF0F0F0F066);//Sends data on this 40-bit address
  SendRadio.write(&value, sizeof(float));
  if (DEBUG) {
   Serial.println(value);
  }
}

void loop() {
  float distances[2];
  if (mpu.update()) {
    static uint32_t prev_ms = millis();
    if (millis() > prev_ms + 25) {
      WriteData(0);
      WriteData(mpu.getYaw());
      WriteData(mpu.getPitch());
      WriteData(mpu.getRoll());
      WriteData(mpu.getAccX());
      WriteData(mpu.getAccY());
      WriteData(mpu.getAccZ());
      // WriteData(distances[0]);
      // WriteData(distances[1]);
    }
  }
}
