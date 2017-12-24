#include <SPI.h>

#include "State.h"
#include "Utils.h"
#include "UMD_Utils.h"

const uint8_t TXLEN = 4;

void setupSPIWithoutPrint() {
  SPISettings settingsA(10000000, MSBFIRST, SPI_MODE1);             ///400000, MSBFIRST, SPI_MODE1);

  SPI.begin();    //AS5047D SPI uses mode=1 (CPOL=0, CPHA=1)
  delay(1000);
  SPI.beginTransaction(settingsA);
}

uint8_t UMDcalculateChecksum(uint8_t stepper_msg_bytearray[]) {
  uint8_t checksum = 0xaa ^ 0x4a; // they are the heading in the message
  for (int i = 0; i < TXLEN - 1; i++) {
    checksum ^= stepper_msg_bytearray[i];
  }
  return checksum;
}

void UMDserialCheck(){
  //SerialUSB.println("Hello, World!");

  StepperMsg stepper_msg;
  if (UMDreadSerialInput(&stepper_msg)==1) { // checksum correct
    UMDinterpretInput(stepper_msg);
  }
  
  //while (SerialUSB.available() <= 0);
  //uint8_t rxdata = SerialUSB.read();
  //SerialUSB.write(rxdata);
  //SerialUSB.println((int)rxdata);
  SerialUSB.write((uint8_t)stepper_msg.flag_period);
  SerialUSB.write((uint8_t)stepper_msg.flag_disable);
  SerialUSB.write((uint8_t)stepper_msg.flag_reverse);
  SerialUSB.write((uint8_t)stepper_msg.stepper_id);
  int myint = stepper_msg.num;
  uint8_t mybyte0 = myint%256;
  uint8_t mybyte1 = myint/256;
  SerialUSB.write(mybyte0);
  SerialUSB.write(mybyte1);

  delay(1000);
}

bool UMDreadSerialInput(StepperMsg* stepper_msg) {
  uint8_t checksum;
  uint8_t rxdata_heading[4] = { 0x01, TXLEN, 0x00, 0x4A };

  for (int i = 0; i < 4; i++) {
    while (SerialUSB.available() <= 0);
    if (SerialUSB.read() != rxdata_heading[i]) {
      return 0;
    }
  }

  // timeout set by Serial.setTimeout() in void setup()
  if (SerialUSB.readBytes((char*)(stepper_msg), 3) != 3){ // serial read time out
    return 0;
  }
  
  while (SerialUSB.available() <= 0);
  checksum = SerialUSB.read();

  if (checksum == UMDcalculateChecksum((uint8_t*) stepper_msg))
    return 1;

  return 0;
}

void UMDinterpretInput(StepperMsg stepper_msg) {
  dir = stepper_msg.flag_reverse;
  int num = stepper_msg.num;
  for (int i=0;i<stepper_msg.num;i++)
    oneStep();
  // stepper_msg.stepper_id no use
  // stepper_msg.flag_period not implemented yet
}
