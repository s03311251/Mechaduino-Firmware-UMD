#ifndef __UMD_UTILS_H__
#define __UMD_UTILS_H__

struct StepperMsg {
  unsigned int flag_disable :1;
  unsigned int flag_period :1;
  unsigned int flag_reverse :1;
  unsigned int stepper_id :2;
  unsigned int num :19; // flag_period == 0 ? no. of pulse : period of pulse
};

void setupSPIWithoutPrint();

uint8_t UMDcalculateChecksum(uint8_t stepper_msg_bytearray[]);
void UMDserialCheck();
bool UMDreadSerialInput(StepperMsg* stepper_msg);
void UMDinterpretInput(StepperMsg stepper_msg);

#endif







