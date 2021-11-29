
#include <Arduino.h>
#include "DCCpp.h"
#include "Config.h"

#ifndef _MUX_CARD_H
#define _MUX_CARD_H

//----------------------------------------------------------------------
// cartes multiplexage
//----------------------------------------------------------------------
typedef struct
{
  byte ENABLE_PIN;  
  byte NB_OUT;
} Mux_Card;

typedef struct
{
  byte card_num;
  byte card_out;
} Max_Card_Out;

void mux_card_init();
byte mux_card_read(byte num_card, byte out);
void mux_card_write(byte num, byte out, byte level);
void selectMuxPin(byte pin, byte maxSIG);
void selectMuxPin8(byte pin);
void selectMuxPin16(byte pin);
void user_mux_card_write(byte pinout, byte value);

#endif