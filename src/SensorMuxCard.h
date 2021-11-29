/**********************************************************************

SensorMuxCard.h
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#ifndef SensorMuxCard_h
  #define S88SensorMuxCard_h_h

  #include "DCCpp.h"

 #ifdef USE_SENSORMUXCARD
   #include "Arduino.h"
   #include "mux_card.h"

struct SensorMuxCard{
   static void SensorInit();
   static void SensorMuxCardCheck();
   static String getSensorStatus();
};

  #endif  // USE_SENSORMUXCARD
#endif  // SensorMuxCard_h