#include <SPI.h>
#include<Wire.h>
#include "RF24.h"
#include "printf.h"
RF24 ReceiveRadio (9, 10);

float values[6];
int qtd;
void ReadData()
{
  if (qtd == 6) qtd = 0;
  uint8_t bytes;
  if (ReceiveRadio.available())
  {
    while (ReceiveRadio.available())
    { 
      bytes = ReceiveRadio.getPayloadSize();
      ReceiveRadio.read(&(values[qtd]), sizeof(float));
      if (values[qtd] == 0) {
        qtd = 0;
        return;
      }
    }
    qtd++;
    Serial.print("yaw ");
    Serial.println(values[0]);
    Serial.print("pitch ");
    Serial.println(values[1]);
    Serial.print("roll ");
    Serial.println(values[2]);
    Serial.print("acc x");
    Serial.println(values[3]);
    Serial.print("acc Y");
    Serial.println(values[4]);
    Serial.print("acc Z");
    Serial.println(values[5]);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  printf_begin();
  Serial.println(F("LGT RF-NANO v2.0 Receive Test"));

  //
  // Setup and configure rf radio
  //
  ReceiveRadio.begin();
  ReceiveRadio.setAddressWidth(5);
  ReceiveRadio.openReadingPipe(1, 0xF0F0F0F066LL);
  ReceiveRadio.setChannel(115);  //115 band above WIFI signals
  ReceiveRadio.setPALevel(RF24_PA_MAX); //MIN power low rage
  ReceiveRadio.setDataRate(RF24_1MBPS) ;  //Minimum speed
  ReceiveRadio.startListening();
  Serial.println("Receive Setup Initialized");
  ReceiveRadio.printDetails();
  delay(500);
}

void loop() {
  ReadData();
}
