/*****************************************************************
File:         readIR.ino
Description:
             1.set debounce value、trigger threshold、response time 、emission current.
             2.read the proximity signal value 、 debounce value、trigger threshold、
               response time、emission current and display on serial monitor.
             3.If you choose to plug the BMS36T001 expansion board in-line into the Arduino,
               select Serial for the communication interface, and comment out the content 
               related to the serial port monitor
connection method： intPin:D3 rxPin:D5 txPin:D4
******************************************************************/
#include "BMS36T001.h"
uint8_t irstatus = 0;
uint8_t irThreshold = 0;
uint8_t irDebounce = 0;
uint8_t irResponseTime = 0;
uint8_t irCurrent = 0;
int16_t  irsignalvalue = 0;
BMS36T001     BMS36(3,5,4); //intPin,rxPin,txPin,,Please comment out this line of code if you don't use SW Serial
//BMS36T001     BMS36(22,&Serial1); //Please uncomment out this line of code if you use HW Serial1 on BMduino
//BMS36T001     BMS36(25,&Serial2); //Please uncomment out this line of code if you use HW Serial2 on BMduino
//BMS36T001     BMS36(3,&Serial3); //Please uncomment out this line of code if you use HW Serial3 on BMduino
//BMS36T001     BMS36(3,&Serial4); //Please uncomment out this line of code if you use HW Serial4 on BMduino
void setup()
{
  BMS36.begin();       
  Serial.begin(9600);            //Set the communication rate between the serial monitor and BMS36T001 to 9600 baud rate    
  BMS36.setIRThreshold(160);      //Set trigger threshold to 160 
  BMS36.setIRDebounce(15);        //Set the number of debounces to 2 
  BMS36.setIRResponseTime(3);    //Set infrared detection response speed to 6 (500ms)
  BMS36.setIRCurrent(80);        //Set the current value of infrared tube emission to 25 (25*5+5=130mA)
}

void loop() 
{ 
  if(!BMS36.getINT())   //Get the module close sensing state 0:not approach 1:approach
  {     
   irThreshold = BMS36.getIRThreshold();   //Get proximity sensor state
   Serial.print("irThreshold:");    
   Serial.println(irThreshold);     
   irDebounce = BMS36.getIRDebounce();    //Gets the number of debounces
   Serial.print("irDebounce:");    
   Serial.println(irDebounce);     
   irResponseTime = BMS36.getIRResponseTime();  //Gets the scan period
   Serial.print("irResponseTime:");    
   Serial.println(irResponseTime);     
   irCurrent = BMS36.getIRCurrent();      //gets the transmit current
   Serial.print("irCurrent:");    
   Serial.println(irCurrent);    
   irsignalvalue = BMS36.readIRSignalValue();  //Gets the signal value
   Serial.print("irsignalvalue:");           
   Serial.println(irsignalvalue);  
   Serial.println();      
  } 
}
