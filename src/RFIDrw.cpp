/**********************************************************************

RFIDrw.cpp
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#include "RFIDrw.h"

long int RFIDrw::sampleTime = 0;

void RFIDrw::init() {
    #ifdef RFID_MUX_CARD
       MuxCard::init();
    #endif
     //OldSensorStatus = SensorMuxCard::getSensorStatus();
}

boolean RFIDrw::checkTime() {
  if (millis() - RFIDrw::sampleTime < RFID_SAMPLE_TIME) // no need to check yet
    return (false);
  RFIDrw::sampleTime = millis();                       // note millis() uses TIMER-0.
  return (true);
} 


void RFIDrw::check() {

}


 
