/**********************************************************************

RFIDrw.cpp
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#include "RFIDrw.h"
//#include "mcp2515.h"

long int RFIDrw::sampleTime = 0;

#define RFIDrw_DEBUG

uint8_t RFCpageAddr = 0x04;      //Les données sont ecrites dans 4 pages : 4,5,6 et 7.
  
unsigned char status;
unsigned char str[MAX_LEN];

RFID _rfid(SS,RST);


#ifdef RFIDrw_DEBUG
  void ShowCardType(unsigned char * type)
  {
    Serial.print("Card type: ");
    if(type[0]==0x04&&type[1]==0x00) 
      Serial.println("MFOne-S50");
    else if(type[0]==0x02&&type[1]==0x00)
      Serial.println("MFOne-S70");
    else if(type[0]==0x44&&type[1]==0x00)
      Serial.println("MF-UltraLight");
    else if(type[0]==0x08&&type[1]==0x00)
      Serial.println("MF-Pro");
    else if(type[0]==0x44&&type[1]==0x03)
      Serial.println("MF Desire");
    else
      Serial.println("Unknown");
  }
#endif


void RFIDrw::init() 
{
    #ifdef RFID_MUX_CARD
       MuxCard::init();
    #endif

    SPI.begin();
    _rfid.init(); 
}

boolean RFIDrw::checkTime() 
{
  if (millis() - RFIDrw::sampleTime < RFID_SAMPLE_TIME) // no need to check yet
    return (false);
  RFIDrw::sampleTime = millis();                       // note millis() uses TIMER-0.
  return (true);
} 


void RFIDrw::check() 
{

//Search card, return card types
  if (_rfid.findCard(PICC_REQIDL, str) == MI_OK) 
  {
    #ifdef RFIDrw_DEBUG
      Serial.println("Find the card!");
      // Show card type
      ShowCardType(str);
    #endif

    if (_rfid.anticoll(str) == MI_OK) 
    {
      #ifdef RFIDrw_DEBUG
        Serial.print("The card's number is  : ");
        for(int i = 0; i < 4; i++)
        {
          Serial.print(0x0F & (str[i] >> 4),HEX);
          Serial.print(0x0F & str[i],HEX);
        }
        Serial.println("");
      #endif

      // lecture numero et nom du train/loco
      // Nous utilisons les pages 4 à 7 inclus (4 octets/page donc 16 octets)
      byte sector         = 1;
      byte blockAddr      = RFCpageAddr;
      byte trailerBlock   = RFCpageAddr + 3;
      byte buffer[18];        // 16 caractères + CRC
      byte size = sizeof(buffer);
      _rfid.read(blockAddr, buffer);

      #ifdef RFIDrw_DEBUG
        // Affichage du contenu du secteur 
        Serial.print(F("Contenu des pages: "));Serial.print(blockAddr);Serial.print(" à ");Serial.println(trailerBlock);
        dump_byte_array(buffer, 16); Serial.println();
        dump_char_array(buffer, 16); Serial.println();
        Serial.println();
      #endif
    }
  
    _rfid.selectTag(str);
  }
  _rfid.halt();
}

 


