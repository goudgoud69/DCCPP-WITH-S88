#ifndef _MUX_CARD_H
#define _MUX_CARD_H

#include <Arduino.h>

#define MUX_CARD_8_PORTS 8
#define MUX_CARD_16_PORTS 16

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