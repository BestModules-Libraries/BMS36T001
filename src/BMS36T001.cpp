/*****************************************************************
File:        BMS36T001.cpp
Author:      XIAO, BESTMODULES
Description: UART communication with the BMS36T001 and obtain the corresponding value  
History:         
V1.0.0   -- initial version；2023-01-04；Arduino IDE : v1.8.16
******************************************************************/
#include "BMS36T001.h"
#include <SoftwareSerial.h>
/**********************************************************
Description: Constructor
Parameters:  intPin: INT Output pin connection with Arduino, the INT will be pulled down when an object approaches
             *theSerial: Wire object if your board has more than one UART interface      
                         parameter range:
                                         BMduino UNO: &Serial、&Serial1、&Serial2、&Serial3、&Serial4
                                         Arduino UNO：&Serial
Return:          
Others:     
**********************************************************/
BMS36T001::BMS36T001(uint8_t intPin,HardwareSerial *theSerial)
{
     _softSerial = NULL;
     _intPin = intPin;
     _hardSerial = theSerial;
}

/**********************************************************
Description: Constructor
Parameters:  intPin: INT Output pin connection with Arduino, 
                     the INT will be pulled down when an object approaches
             rxPin: Receiver pin of the UART
             txPin: Send signal pin of UART         
Return:          
Others:   
**********************************************************/
BMS36T001::BMS36T001(uint8_t intPin,uint16_t rxPin,uint16_t txPin)
{
    _hardSerial = NULL;
    _intPin = intPin;
    _rxPin = rxPin;
    _txPin = txPin;
    _softSerial = new SoftwareSerial(_rxPin,_txPin);
}

/**********************************************************
Description: Module Initial
Parameters:  baudRate: Module communication baud rate(Unique 9600bps)        
Return:          
Others:   If the hardware UART is initialized, the _softSerial 
          pointer is null, otherwise it is non-null       
**********************************************************/
void BMS36T001::begin(uint16_t baud)
{
    
    if(_softSerial != NULL)
    {
      pinMode(_intPin,INPUT);
      _softSerial->begin(baud); 
    }
    else
    {
      pinMode(_intPin,INPUT);
      _hardSerial->begin(baud);
    }
}
/**********************************************************
Description: Get INT Status
Parameters:          
Return:      INT Status：
                        0:INT output low level 
                        1:INT output high level  
Others:     
**********************************************************/
uint8_t BMS36T001::getINT()
{
    if(_softSerial != NULL)
    {
        return (digitalRead(_intPin));  
    }
    else
    {
        return (digitalRead(_intPin));
    }
}

/**********************************************************
Description: Get IR Induction state
Parameters:         
Return:      irStatus: 1:approach   0:not approach    
Others:      
**********************************************************/

uint8_t BMS36T001::getIRStatus()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x02, 0x01,0xD8};
    uint8_t irStatus = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     irStatus = buff[4];
    }
    delay(10);
    return irStatus;
}

/**********************************************************
Description: Read  signal value
Parameters:         
Return:      signalValue: irEnv - irRef     
Others:      
**********************************************************/
int16_t BMS36T001::readIRSignalValue()
{
    int16_t signalValue = 0;
    uint8_t irRef = 0;
    uint8_t irEnv = 0;
    irRef = readIRRef();
    irEnv = readIREnv();
    signalValue = irEnv - irRef;
    return signalValue;
}

/**********************************************************
Description: IR motor standby command
Parameters:         
Return:      0:Success 1:Fail     
Others:      OUT1 -> the positive pole of the motor  =  HIGH_Z
             OUT2 -> the negative pole of the motor  =  LOW
**********************************************************/
uint8_t BMS36T001::motorStandby()
{    
    uint8_t sendBuf[3] = {0x55, 0x20, 0x75}; 
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,3);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: IR motor Reverse command
Parameters:         
Return:      0:Success 1:Fail     
Others:      OUT1 -> the positive pole of the motor  =  LOW
             OUT2 -> the negative pole of the motor  =  HIGH
**********************************************************/
uint8_t BMS36T001::motorReverse()
{     
    uint8_t sendBuf[3] = {0x55, 0x21, 0x76}; 
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,3);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: IR motor Forward command
Parameters:         
Return:      0:Success 1:Fail     
Others:      OUT1 -> the positive pole of the motor  =  HIGH
             OUT2 -> the negative pole of the motor  =  LOW
**********************************************************/
uint8_t BMS36T001::motorForward()
{
    uint8_t sendBuf[3] = {0x55, 0x22, 0x77}; 
    uint8_t buff[3] = {0};
    writeBytes(sendBuf,3);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: IR motor Break command
Parameters:         
Return:      0:Success 1:Fail     
Others:      OUT1 -> the positive pole of the motor  = LOW 
             OUT2 -> the negative pole of the motor  = LOW
**********************************************************/
uint8_t BMS36T001::motorBreak()
{     
   uint8_t sendBuf[3] = {0x55, 0x23, 0x78}; 
   uint8_t buff[3] = {0};
   writeBytes(sendBuf,3);
   if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Save function parameters to EEPROM
Parameters:         
Return:      0:Success 1:Fail     
Others:      Writes the current parameter to EEPROM
**********************************************************/
uint8_t BMS36T001::saveSettings()
{
   uint8_t sendBuf[3] = {0x55, 0x1A, 0x6F}; 
   uint8_t buff[3] = {0};
   writeBytes(sendBuf,3);
   delay(40);
   if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Get IR trigger threshold
Parameters:         
Return:      threshold: Trigger threshold(16~180)   
Others:      
**********************************************************/
uint8_t BMS36T001::getIRThreshold()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x08, 0x01,0xDE};
    uint8_t threshold = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     threshold = buff[4];
    }
    delay(10);
    return threshold;
}

/**********************************************************
Description: Get IR debounce value
Parameters:         
Return:      debounce: Number of debounces(0~15)   
Others:      
**********************************************************/
uint8_t BMS36T001::getIRDebounce()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x09, 0x01,0xDF};
    uint8_t debounce = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     debounce = buff[4];
    }
    delay(10);
    return debounce;
}

/**********************************************************
Description: Get IR response time 
Parameters:         
Return:      responseTime: 
                          0x00:SPEED_8_MS       
                          0x01:SPEED_16_MS      
                          0x02:SPEED_32_MS       
                          0x03:SPEED_64_MS      
                          0x04:SPEED_128_MS      
                          0x05:SPEED_256_MS      
                          0x06:SPEED_500_MS
                          0x07:SPEED_1_S  
                          0x08:SPEED_FASTMODE    
Others:      
**********************************************************/
uint8_t BMS36T001::getIRResponseTime()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x0A, 0x01,0xE0};
    uint8_t responseTime = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     responseTime = buff[4];
    }
    delay(10);
    return responseTime;
}

/**********************************************************
Description: Get IR emission current value 
Parameters:         
Return:      current: 
                      0~ 63,5 mA/step, first order amplification  
                      64~ 127,5 mA/step, two-stage amplification  
                      The calculation formula of current value is as follows:  
                      Dn >= 63, IEmmision = (Dn - 64) x 5 + 5  
                      Dn <= 63 or less, IEmmision = Dn x 5 + 5        
Others:      
**********************************************************/
uint8_t BMS36T001::getIRCurrent()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x0D, 0x01,0xE3};
    uint8_t current = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     current = buff[4];
    }
    delay(10);
    return current;
}

/**********************************************************
Description: Set IR trigger threshold
Parameters:  threshold: Trigger threshold
                        parameter range:16~180(Default 16)      
Return:      0:Success 1:Fail    
Others:      
**********************************************************/
uint8_t BMS36T001::setIRThreshold(uint8_t threshold)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x08, 0x01, 0x00, 0x00};
    uint8_t buff[3] = {0};
    sendBuf[4] = threshold;
    sendBuf[5] = 30+threshold;
    writeBytes(sendBuf,6); 
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description:  Set IR debounce value
Parameters:  debounce: Number of debounces
                       parameter range:0~15(Default 0)      
Return:      0:Success 1:Fail    
Others:      
**********************************************************/
uint8_t BMS36T001::setIRDebounce(uint8_t  debounce)
{
   uint8_t sendBuf[6] = {0x55, 0xC0, 0x09, 0x01, 0x00, 0x00};
   uint8_t buff[3] = {0};
   sendBuf[4] = debounce;
   sendBuf[5] = 31+debounce;
   writeBytes(sendBuf,6);
   if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}


/**********************************************************
Description: Set IR response time
Parameters:  response time: 0x00~0x08 (Default 0x06)
             Optional:
                      0x00:SPEED_8_MS       
                      0x01:SPEED_16_MS      
                      0x02:SPEED_32_MS       
                      0x03:SPEED_64_MS      
                      0x04:SPEED_128_MS      
                      0x05:SPEED_256_MS      
                      0x06:SPEED_500_MS
                      0x07:SPEED_1_S  
                      0x08:SPEED_FASTMODE        
Return:      0:Success 1:Fail 
Others:      
**********************************************************/
uint8_t BMS36T001::setIRResponseTime(uint8_t  responseTime)
{
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x0A, 0x01, 0x00, 0x00};
    uint8_t buff[3] = {0};
    sendBuf[4] = responseTime;
    sendBuf[5] = 32+responseTime;
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}


/**********************************************************
Description: Set IR emission current value
Parameters:  current:Emission current parameters  
                  parameter range: 
                                  0~ 63,5 mA/step, first order amplification  
                                  64~ 127,5 mA/step, two-stage amplification  
                                  The calculation formula of current value is as follows:  
                                  Dn >= 63, IEmmision = (Dn - 64) x 5 + 5  
                                  Dn <= 63 or less, IEmmision = Dn x 5 + 5       
Return:      0:Success 1:Fail            
Others:      
**********************************************************/
uint8_t BMS36T001::setIRCurrent(uint8_t  current)
{ 
    uint8_t sendBuf[6] = {0x55, 0xC0, 0x0D, 0x01, 0x00, 0x00};
    uint8_t buff[3] = {0};
    sendBuf[4] = current;
    sendBuf[5] = 35+current;
    writeBytes(sendBuf,6);
    if(readBytes(buff,3)== CHECK_OK)
    {
     if(buff[1]== 0x7f)
     {
       delay(10);
       return SUCCESS;
     }
    }
    delay(10);
    return FAIL ;
}

/**********************************************************
Description: Get reference value 
Parameters:         
Return:      IrRef:Proximity sensing reference value. (Sensing value when IR LED is not on)      
Others:      
**********************************************************/
uint8_t BMS36T001::readIRRef()
{
    uint8_t sendBuf[5] = {0x55, 0x80, 0x03, 0x01,0xD9};
    uint8_t IrRef = 0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     IrRef = buff[4];
    }
    delay(10);
    return IrRef;
}

/**********************************************************
Description: Get environmental value 
Parameters:         
Return:      IREnv:Proximity sensing environmental values. (Sensing value when IR LED is turned on )     
Others:      
**********************************************************/
uint8_t BMS36T001::readIREnv()
{   
    uint8_t sendBuf[5] = {0x55, 0x80, 0x04, 0x01,0xDA};
    uint8_t IREnv =  0;
    uint8_t buff[6] = {0};
    writeBytes(sendBuf,5);
    if(readBytes(buff,6)== CHECK_OK)
    {
     IREnv = buff[4];
    }
    delay(10);
    return IREnv;
}


/**********************************************************
Description: writeBytes
Parameters:  wbuf[]:Variables for storing Data to be sent
             wlen:Length of data sent  
Return:   
Others:
**********************************************************/
void BMS36T001::writeBytes(uint8_t wbuf[], uint8_t wlen)
{
  /* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    while (_softSerial->available() > 0)
    {
      _softSerial->read();
    }
    _softSerial->write(wbuf, wlen);
  }
  /* Select HardwareSerial Interface */
  else
  {
    while (_hardSerial->available() > 0)
    {
      _hardSerial->read();
    }
    _hardSerial->write(wbuf, wlen);
  }
}

/**********************************************************
Description: readBytes
Parameters:  rbuf[]:Variables for storing Data to be obtained
             rlen:Length of data to be obtained
Return:   
Others:
**********************************************************/
uint8_t BMS36T001::readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut)
{
  uint8_t i = 0, delayCnt = 0, checkSum = 0;
/* Select SoftwareSerial Interface */
  if (_softSerial != NULL)
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_softSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _softSerial->read();
    }
  }
/* Select HardwareSerial Interface */
  else
  {
    for (i = 0; i < rlen; i++)
    {
      delayCnt = 0;
      while (_hardSerial->available() == 0)
      {
        if (delayCnt > timeOut)
        {
          return TIMEOUT_ERROR; // Timeout error
        }
        delay(1);
        delayCnt++;
      }
      rbuf[i] = _hardSerial->read();
    }
  }

  /* check Sum */
  for (i = 0; i < (rlen - 1); i++)
  {
    checkSum += rbuf[i];
  }
  if (checkSum == rbuf[rlen - 1])
  {
    return CHECK_OK; // Check correct
  }
  else
  {
    return CHECK_ERROR; // Check error
  }
}
