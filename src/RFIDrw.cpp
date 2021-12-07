/**********************************************************************

RFIDrw.cpp
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#include "RFIDrw.h"
#include "mcp2515.h"

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
    //RFID _rfid(SS,RST);
    //RFID RFIDrw::rfid(SS,RST); // = _rfid;
    
    #ifdef RFID_MUX_CARD
       MuxCard::init();
    #endif

    SPI.begin();
    _rfid.init(); 

     //OldSensorStatus = SensorMuxCard::getSensorStatus();
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
        //char serialID[5];
        //Serial.write(0x02);
        for(int i = 0; i < 4; i++)
        {
          //serialID[i]=str[i];
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


      send_byte_array_incl_ChkSum(str, 6);     // send card-ID (5 words) to the RS232 port in HEX, including Checksum

     // RFIDrw::sendRFIDDetected(1, buffer);
    }
  
    _rfid.selectTag(str);
  }
  _rfid.halt();
}
/*
void RFIDrw::sendRFIDDetected(byte can_id, byte *buffer) 
{
  struct can_frame canMsg;

  // numero de loco
  byte nloco = (buffer[0] - '0')*10 + (buffer[1] - '0');

  canMsg.can_id = can_id; //0x036; //CAN id as 0x036
  canMsg.can_dlc = 8; //CAN data length as 8
  
  canMsg.data[0] = nloco;
  for (byte i = 1; i < 8; i++)
  { 
    canMsg.data[i] = buffer[i+2];
  }



  MCP2515 mcp2515(0);
  mcp2515.sendMessage(&canMsg); //Sends the CAN message
}
*/

/*** routine to send the UiD including STX, checksum and ETX to Serial.*/
//void send_byte_array_incl_ChkSum(byte *buffer, byte bufferSize) 
void send_byte_array_incl_ChkSum(unsigned char *buffer, byte bufferSize)  
{
  byte i=1, Checksum=0 ;
      
    Serial.write(0x02);                                              // STX (02h). You must send STX as a byte, only then STX is recognized as a command
    
    for (i = 1; i < 6; i++)                                          // Remove first and last byte, we can only manage messages with 5 bytes of data
    {          
      //Serial.print(0x0F & (str[i] >> 4),HEX);
      //Serial.print(0x0F & str[i],HEX);                                                                       
      if (i==1) Checksum  = (byte)buffer[i];
      else      Checksum ^= (byte)buffer[i];   
      if ((byte)buffer[i] < 0x10) Serial.print (0);                        // HEX value is always 2 bytes
      Serial.print((byte)buffer[i],HEX);                                   // send information
    }
   if (Checksum < 0x10) Serial.print (0);                            // HEX value is always 2 bytes
   Serial.println(Checksum,HEX);                                     // send checksum
   Serial.write(0x03);                                               // ETX (03h). You must send ETX as a byte, only then ETX is recognized as a command
}

 


