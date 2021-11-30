/**********************************************************************

  SensorMuxCard.cpp
  COPYRIGHT (c) 2021 David Goudard
  Last update 2021/11/28
  
  http://lormedy.free.fr/DCCpp.html
  http://fadiese.hd.free.fr/cms/index.php?page=dcc

***********************************************************************
 They are provided to ease operations with long layout used by clubs. 
  This sensor data collection will be sent back as a binary or hexadecimal string.
  This feedback will occure as soon as any sensor change is detected or upon request of the control software. 

<Y Nb_S88_Modules DataFormat>  with: Nb_S88_Modules to read = 2..64 (step of 2)
                                       DataFormat = 0 for binary digit answer
                                       DataFormat = 1 for hexadecimal encoded answer for CDM-Rail
                                       DataFormat = 2 for pure hexadecimal encoded answer 
                                       DataFormat = 3 for SENSOR like style use by JMRI & Rocrail
                returns: <y S88status>  and <y 0001001011000000........> or <y 12C0..>
                any sensor change will send new data to the PC

  The 2 data collections have the same length : total Nb_S88_Modules = 2..64 (step of 2). 
  They are concatened into 1 single buffer and send to the PC, DataL followed by DataR.

 - the following variation of the "Q" command:
  <Q>:  sets Nb_S88_Modules to read = 2..64 (step of 2)
                                       DataFormat = 2 for SENSOR like style use by JMRI & Rocrail
        returns: <Q ID> if sensor Id is active (1), <q ID> if sensor ID is inactive (0)
                 any sensor change will send new data to the PC

  Sensor list from 1 to 512 are reserved for S88 bus. Extra sensors should use ID > 512 up to 32768.
  EPRROM storage uses address 2048 to store 2 bytes
  This routine is compatible with train controller softwares CDM-Rail, CDT30, JMRI and Rocrail. 
  Both CDM-Rail and CDT30 are compatible with DCC++ and DCCpp, thus DCCpp_S88.

  Tested with both Lormedy S88-N 8E/16E and Littfinski RM-GB-8-N modules
  
***********************************************************************
*/

#include "DCCpp.h"

#ifdef USE_SENSORMUXCARD
  #include "SensorMuxCard.h"
  #include "mux_card.h"
  #include "DCCpp_Uno.h"
  #include "Comm.h"

  #ifdef VISUALSTUDIO
    #include "string.h"
  #endif

  #ifdef USE_TEXTCOMMAND
    #include "TextCommand.h"
  #endif

  int DataFormat = 3;     // Output DataFormat 0=binary 1=hexa 2=Q ID; 9=disabled
  //String SensorStatus;       //  sensor status response
  String OldSensorStatus;

// Définition des cartes
#if MUX_CARD_NB > 0
Mux_Card mux_card[MUX_CARD_NB] = {
    {2, 16}}; // pin 2, model 16 ports
#endif

void SensorMuxCard::init() {
     mux_card_init();
     OldSensorStatus = SensorMuxCard::getSensorStatus();
}

void SensorMuxCard::check() {
    String SensorStatus;
    if (DataFormat < 3) SensorStatus = "<y ";  // start of feedback (CDT30 or CDM-Rail)
    if (DataFormat == 0) {                  // binary in ASCII
        SensorStatus += SensorMuxCard::getSensorStatus();
    } else if ( (DataFormat == 1) || (DataFormat == 2) ) {           // hexa in ASCII or pure hexa
    } else if (DataFormat == 3) {           // JMRI, Rocrail or SENSOR style
        SensorStatus = SensorMuxCard::getSensorStatus();
         
        if (OldSensorStatus != SensorStatus) {
         INTERFACE.print(SensorStatus);
          for (unsigned int sIndex = 0; sIndex < SensorStatus.length(); sIndex++) {
              String tmp = SensorStatus.substring(sIndex, 1); //sIndex + 1);
              String oldTmp = OldSensorStatus.substring(sIndex, 1); //sIndex + 1);
              if (tmp[0] != oldTmp[0]) {
                INTERFACE.print( tmp[0] == '0' ? "<q " : "<Q " );
                INTERFACE.print(sIndex+1);      // sIndex range 0..511
                INTERFACE.print(">");
#if !defined(USE_ETHERNET)
                INTERFACE.println("");
#endif
              }
          }
          OldSensorStatus = SensorStatus;
        }
#ifdef USE_EEPROM        
        EEStore::data.nSensorMuxCard =  SensorStatus.length();
#endif
    }
    if (DataFormat < 3) {
        SensorStatus += ">";                       // end of feedback (CDT30 or CDM-Rail)
        INTERFACE.print(SensorStatus);           // send automaticly data at each sensors state change
#if !defined(USE_ETHERNET)
        INTERFACE.println("");
#endif
    }
}

String SensorMuxCard::getSensorStatus() {
    String SensorStatus = "";
#if MUX_CARD_NB >= 1
    for (byte i = 0; i < MUX_CARD_NB; i++)
    {
        for (byte j=0; j < mux_card[i].NB_OUT; j++)
          SensorStatus +=  mux_card_read(i, j);
    }
#endif
    return SensorStatus;    
}

#endif  // USE_SENSORMUXCARDs