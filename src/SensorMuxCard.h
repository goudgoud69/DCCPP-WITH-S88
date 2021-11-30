/**********************************************************************

SensorMuxCard.h
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#ifndef SensorMuxCard_h
  #define SensorMuxCard_h

 #ifdef USE_SENSORMUXCARD
   #include "Arduino.h"
   #include "DCCpp.h"
   #include "mux_card.h"

struct SensorMuxCard{
   static void init();
   static void check();
   static String getSensorStatus();
};

  #endif  // USE_SENSORMUXCARD
#endif  // SensorMuxCard_h