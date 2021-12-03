//----------------------------------------------------------------------
// Envoi à la carte de multiplexage
//----------------------------------------------------------------------

/*
Basé sur Analog/digital 16-to-1 multiplexer module (HW-178, CD74HC4067)
CD4051B
*/


#include <Arduino.h>
#include "DCCpp.h"
#include "Config.h"
#include "mux_card.h"

#define DEBUG_MODE

const int mux_selectPins[] = {MUX_S0_PIN, MUX_S1_PIN, MUX_S2_PIN, MUX_S3_PIN};

void MuxCard::init()
{
#if MUX_CARD_NB >= 1
    pinMode(MUX_S0_PIN, OUTPUT);
    pinMode(MUX_S1_PIN, OUTPUT);
    pinMode(MUX_S2_PIN, OUTPUT);

    digitalWrite(MUX_S0_PIN, LOW);
    digitalWrite(MUX_S1_PIN, LOW);
    digitalWrite(MUX_S2_PIN, LOW);
       
    if (MUX_CARD_NB_VOIES == MUX_CARD_16_PORTS)
    {
        pinMode(MUX_S3_PIN, OUTPUT);
        digitalWrite(MUX_S3_PIN, LOW);
    }
    
    // Active le CD4051B
    pinMode(MUX_ENABLE_PIN, OUTPUT);
    digitalWrite(MUX_ENABLE_PIN, LOW);

#endif
}

/**
 * Mux_Card mux_card : mux card
 * byte channel      : n° voie
 * 
 * return byte level    : valeur à écrire 
 */
byte MuxCard::read(Mux_Card mux_card, byte channel)
{
    byte val;

#if MUX_CARD_NB >= 1

    if (channel > mux_card.NB_CHANNEL - 1)
        return 0;

    digitalWrite(MUX_S0_PIN, bitRead(channel, 0));
    digitalWrite(MUX_S1_PIN, bitRead(channel, 1));
    digitalWrite(MUX_S2_PIN, bitRead(channel, 2));

    // force les sorties
    if (mux_card.NB_CHANNEL == MUX_CARD_16_PORTS)
    {
       digitalWrite(MUX_S3_PIN, bitRead(channel, 3));
    }
    delay(2);

    val = digitalRead(mux_card.SIG_IN_PIN); //analogRead(MUX_SIG_IN_PIN); utiliser digital comme cela on reçoit 0 ou 1 pour détecteur présence c'est mieux.
#endif
     
    #ifdef DEBUG_MODE 
    //Serial.print(" "); 
    //Serial.println(val);
    #endif

    return val;
}

/**
 * Mux_Card mux_card : num mux card
 * byte channel      : n° voie
 * byte level    : valeur à écrire 
 */
void MuxCard::write(Mux_Card mux_card, byte channel, byte level)
{

    if (channel > mux_card.NB_CHANNEL - 1)
        return;

    digitalWrite(MUX_S0_PIN, bitRead(channel, 0));
    digitalWrite(MUX_S1_PIN, bitRead(channel, 1));
    digitalWrite(MUX_S2_PIN, bitRead(channel, 2));

    // force les sorties
    if (mux_card.NB_CHANNEL == MUX_CARD_16_PORTS)
    {
       digitalWrite(MUX_S3_PIN, bitRead(channel, 3));
    }
    delay(2);

    digitalWrite(mux_card.SIG_IN_PIN, level); 
}