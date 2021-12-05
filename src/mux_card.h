#ifndef _MUX_CARD_H
#define _MUX_CARD_H

#include <Arduino.h>

#define MUX_CARD_8_PORTS 8
#define MUX_CARD_16_PORTS 16

////////////////////////////////////////////////////////////////////////////
// SELECT PINS MULTIPLEX CARD
////////////////////////////////////////////////////////////////////////////
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO)      // Configuration for UNO/NANO
  #define MUX_ENABLE_PIN 2
  #define MUX_S0_PIN 6
  #define MUX_S1_PIN 7
  #define MUX_S2_PIN 8
  #define MUX_S3_PIN 9

  #define MUX_CARD_PIN_1 A2
  #define MUX_CARD_PIN_2 A4
 #else     
  #define MUX_ENABLE_PIN 5                                    // Configuration for MEGA
  #define MUX_S0_PIN 6
  #define MUX_S1_PIN 7
  #define MUX_S2_PIN 8
  #define MUX_S3_PIN 9

  #define MUX_CARD_PIN_1 A2
  #define MUX_CARD_PIN_2 A4
 #endif  


//----------------------------------------------------------------------
// cartes multiplexage
//----------------------------------------------------------------------
typedef struct
{
  byte SIG_IN_PIN;
  byte NB_CHANNEL;
} Mux_Card;

typedef struct
{
  byte card_num;
  byte card_out;
} Max_Card_Out;

struct MuxCard 
{
  static void init();
  static byte read(Mux_Card mux_card, byte out);
  static void write(Mux_Card mux_card, byte out, byte level);
//void selectMuxPin(byte pin, byte maxSIG);
//void selectMuxPin8(byte pin);
//void selectMuxPin16(byte pin);
  static void user_write(byte pinout, byte value);
};

#endif