/*****************************************************************
File:             BMS36T001.h
Author:           BESTMODULES
Description:      Define classes and required variables
Version:          V1.0.2   -- 2023-09-13         
******************************************************************/

#ifndef _BMS36T001_H_
#define _BMS36T001_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

#define   BMS36T001_BAUD    9600
#define   SUCCESS           0
#define   FAIL              1
#define   CHECK_OK        0
#define   CHECK_ERROR     1
#define   TIMEOUT_ERROR   2

/*IR detection of reaction speed*/
#define   SPEED_8_MS       (0x00)
#define   SPEED_16_MS      (0x01)
#define   SPEED_32_MS      (0x02)
#define   SPEED_64_MS      (0x03)
#define   SPEED_128_MS     (0x04)
#define   SPEED_256_MS     (0x05)
#define   SPEED_500_MS     (0x06)
#define   SPEED_1_S        (0x07)
#define   SPEED_FASTMODE   (0x08)


class BMS36T001
{
    public:
    BMS36T001(uint8_t intPin,HardwareSerial *theSerial  = &Serial);
    BMS36T001(uint8_t intPin,uint16_t rxPin,uint16_t txPin);
    void begin(uint16_t baud = BMS36T001_BAUD);
    uint8_t getINT();
    uint8_t getIRStatus();
    int16_t readIRSignalValue();
    uint8_t saveSettings();
    int16_t getFWVer();
    uint8_t motorForward();
    uint8_t motorReverse();
    uint8_t motorBreak();
    uint8_t motorStandby();
    
    uint8_t getIRThreshold();   
    uint8_t getIRDebounce();
    uint8_t getIRResponseTime();
    uint8_t getIRCurrent();
    uint8_t setIRThreshold(uint8_t threshold = 16);
    uint8_t setIRDebounce(uint8_t debounce = 0);
    uint8_t setIRResponseTime(uint8_t responseTime = SPEED_500_MS);
    uint8_t setIRCurrent(uint8_t current);
    
    
    private:
    uint8_t  readIRRef();
    uint8_t  readIREnv();
    void writeBytes(uint8_t wbuf[], uint8_t wlen);
    uint8_t readBytes(uint8_t rbuf[], uint8_t rlen, uint16_t timeOut = 10);

    uint8_t _intPin;
    uint8_t _rxPin;
    uint8_t _txPin;
    HardwareSerial *_hardSerial = NULL;
    SoftwareSerial *_softSerial = NULL ;  
};
#endif
