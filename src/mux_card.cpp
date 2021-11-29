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

extern Mux_Card mux_card[];

const int mux_selectPins[] = {MUX_S0_PIN, MUX_S1_PIN, MUX_S2_PIN, MUX_S3_PIN};

void mux_card_init()
{
#if MUX_CARD_NB >= 1
    for (byte i = 0; i < MUX_CARD_NB; i++)
    {
        // Active le CD4051B
        pinMode(mux_card[i].ENABLE_PIN, OUTPUT);
        digitalWrite(mux_card[i].ENABLE_PIN, LOW);

        pinMode(MUX_S0_PIN, OUTPUT);
        pinMode(MUX_S1_PIN, OUTPUT);
        pinMode(MUX_S2_PIN, OUTPUT);

        digitalWrite(MUX_S0_PIN, LOW);
        digitalWrite(MUX_S1_PIN, LOW);
        digitalWrite(MUX_S2_PIN, LOW);

        if (mux_card[i].NB_OUT % 8 == 2)
        {
            pinMode(MUX_S3_PIN, OUTPUT);
            digitalWrite(MUX_S3_PIN, LOW);
        }

        // Désactive le CD4051B
        pinMode(mux_card[i].ENABLE_PIN, OUTPUT);
        digitalWrite(mux_card[i].ENABLE_PIN, HIGH);
    }

#endif
}

/**
 * byte num_card : num mux card
 * byte out      : n° voie
 * 
 * return byte level    : valeur à écrire 
 */
byte mux_card_read(byte num_card, byte out)
{
    byte val;

#if MUX_CARD_NB >= 1
    if (num_card > MUX_CARD_NB - 1)
        return 0;

    for (byte i = 0; i < MUX_CARD_NB; i++)
    {
        //desactive le CD4051B
        pinMode(mux_card[i].ENABLE_PIN, OUTPUT);
        digitalWrite(mux_card[i].ENABLE_PIN, HIGH);
    }


    // Active le CD4051B
    pinMode(mux_card[num_card].ENABLE_PIN, OUTPUT);
    digitalWrite(mux_card[num_card].ENABLE_PIN, LOW);

    //pinMode(MUX_SIG_IN_PIN, INPUT);

    // force les sorties
    if (mux_card[num_card].NB_OUT > 8)
    {
        selectMuxPin16(out);
    }
    else
    {
        selectMuxPin8(out);
    }
    val = digitalRead(MUX_SIG_IN_PIN); //analogRead(MUX_SIG_IN_PIN); utiliser digital comme cela on reçoit 0 ou 1 pour détecteur présence c'est mieux.
#endif

    return val;
}

// The selectMuxPin function sets the S0, S1, and S2 pins to select the give pin
void selectMuxPin8(byte pin)
{
    if (pin > 7)
        return; // Exit if pin is out of scope
    selectMuxPin(pin, 3);
}

void selectMuxPin16(byte pin)
{
    if (pin > 15)
        return; // Exit if pin is out of scope
    selectMuxPin(pin, 4);
}

void selectMuxPin(byte pin, byte maxSIG)
{
    for (int i = 0; i < maxSIG; i++)
    {
        /*
        if (DEBUG_MODE) {
            Serial.print("Pin ");
            Serial.print(mux_selectPins[i]);
            Serial.print("-->");
            Serial.println(bitRead(pin, i));
        }
        */
        digitalWrite(mux_selectPins[i], bitRead(pin, i));
    }
}