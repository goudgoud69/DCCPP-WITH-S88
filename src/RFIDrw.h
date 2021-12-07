/**********************************************************************

RFIDrw.h
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#ifndef RFIDrw_h
  #define RFIDrw_h

  #include "Config.h"
/*
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 * 
 * 
 */

  //#define SS 53
  //#define MOSI 51
  //#define MISO 50
  //#define SCK 52
  #define RST 49

  #define RFID_SAMPLE_TIME 125
  //#define RFID_MUX_CARD


  #include <SPI.h>
  #include <RFID.h>
  #include "mux_card.h"

   struct RFIDrw {
        static long int sampleTime;
        static boolean checkTime();
        static void init();
        static void check();
        //RFID rfid;
        static Mux_Card mux_card;
        static void sendRFIDDetected(byte can_id, byte *buffer);
    };

    void send_byte_array_incl_ChkSum(unsigned char *buffer, byte bufferSize);

#endif

