/**********************************************************************

RFIDrw.h
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#ifndef RFIDrw_h
  #define RFIDrw_h
    #include <SPI.h>
    #include <RFID.h>

  #ifdef USE_RFID
    struct RFIDdw {
        static boolean checkTime();
        static void init();
        static void check();
        RFID rfid[];

    };
  #endif
#endif

