/**********************************************************************

SensorMuxCard.h
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#ifndef SensorMuxCard_h
#define SensorMuxCard_h

  

 #ifdef USE_SENSORMUXCARD
   #include "mux_card.h"

  #define SENSORMUXCARD_MUX_CARD_NB 2 // nombre de carte multiplexeurs/demultiplexeurs
  #define SENSORMUXCARD_SAMPLE_TIME 500

  struct SensorMuxCard{
    static long int sampleTime; 
    static Mux_Card mux_card[];
    static boolean checkTime();

    static void init();
    static void check();
    static String getSensorStatus();
  };

  #endif  // USE_SENSORMUXCARD
#endif  // SensorMuxCard_h