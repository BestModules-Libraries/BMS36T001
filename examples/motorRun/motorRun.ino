/*****************************************************************
File:         motorRun.ino
Description:  
              1.Obtain IR state and control motor running state through 
              UART and display the value in the serial port.
              2.If the object is sensed, it will print the motor forward and 
              control the motor forward. 
              3.If there is no sensing object,it will print the motor standby
              and control the motor in standby state.
              4.Drive motor capacity: 24V 1.2A
connection method： 
                  1.According to the motor operating voltage, the power supply 
                    is connected externally to the + - port of the module
                  2.Out1 is connected to the positive pole of the motor 
                  3.Out2 is connected to the negative pole of  the motor
                  4.intPin:D2 rxPin:D5 txPin:D4
******************************************************************/
#include "BMS36T001.h"
uint8_t irstatus = 0;
//BMS36T001     BMS36(3,5,4); //intPin,rxPin,txPin,,Please comment out this line of code if you don't use SW Serial
BMS36T001     BMS36(2,&Serial); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMS36T001     BMS36(2,&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMS36T001     BMS36(2,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMS36T001     BMS36(2,&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMS36T001     BMS36(2,&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino
void setup()
{
  BMS36.begin(); 
}

void loop() 
{ 
  irstatus = BMS36.getIRStatus();
  if(irstatus)  //Get the module close sensing state 0:not approach 1:approach
  {
    BMS36.motorForward();    // motor forward 
 } 
  else
  {
    BMS36.motorStandby();  // motor standby
  }
}
 
