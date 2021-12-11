/**********************************************************************

RFIDrw.cpp
COPYRIGHT (c) 2021 David Goudard

**********************************************************************/

#include "RFIDrw.h"
#include "can-serial.h"



long int RFIDrw::sampleTime = 0;

//#define RFIDrw_DEBUG

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
     Can232::init();

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
        str[0]=0;
        for(int i = 1; i < 5; i++)
        {
          //serialID[i]=str[i];
          Serial.print(0x0F & (str[i] >> 4), HEX);
          Serial.print(0x0F & str[i], HEX);
        }
        Serial.println("");
      #endif
     
/*
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
*/

      //send_byte_array_incl_ChkSum(str, 6);     // send card-ID (5 words) to the RS232 port in HEX, including Checksum

     RFIDrw::sendRFIDDetected(1, str);
    }
  
    _rfid.selectTag(str);
  }
  _rfid.halt();
}

void RFIDrw::sendRFIDDetected(byte rfidReader, unsigned char *buffer) 
{
  CANMsg canMsg;

  // numero de loco
  //byte nloco = (buffer[0] - '0')*10 + (buffer[1] - '0');

  canMsg.b[d0] = OPC_DDES; //OPC_ACDAT; //OPC_DDES; 
  canMsg.b[d1] = (rfidReader / 256) & 0xFF;
  canMsg.b[d2] = (rfidReader) & 0xFF;
  canMsg.b[d3] = buffer[0];
  canMsg.b[d4] = buffer[1];
  canMsg.b[d5] = buffer[2];
  canMsg.b[d6] = buffer[3];
  canMsg.b[d7] = buffer[4];
  canMsg.b[dlc] = 8;
            
send_byte_array_incl_ChkSum(canMsg);



/*

  canMsg.can_id = can_id; //0x036; //CAN id as 0x036
  canMsg.can_dlc = 8; //CAN data length as 8
  
  canMsg.data[0] = nloco;
  for (byte i = 1; i < 8; i++)
  { 
    canMsg.data[i] = buffer[i+2];
  }
*/


  //MCP2515 mcp2515(0);
  //mcp2515.sendMessage(&canMsg); //Sends the CAN message
}



/*** routine to send the UiD including STX, checksum and ETX to Serial.*/
//void send_byte_array_incl_ChkSum(byte *buffer, byte bufferSize) 

/*
[STX]
[D1] [D2] [D3] [D4] [D5] [D6] [D7] [D8] [D9] [D10]
[CS1] [CS2]
[CR]
[LF]
[ETX]
*/
void send_byte_array_incl_ChkSum(CANMsg canMsg)  
{
  byte i=1;
  unsigned char Checksum=0 ;
      
  canMsg.b[con] = 0; // ou OPC_ACDAT ?

  canMsg.b[sidh] = (CANID >> 3);
  canMsg.b[sidl] = (CANID << 5);

  canMsg.b[eidh] = 0x00;
  canMsg.b[eidl] = 0x00;

  /*
  Serial.write(":");
  Serial.write("S");

  Serial.print(0x0F & (canMsg.b[sidh] >> 4), HEX);
  Serial.print(0x0F & canMsg.b[sidh], HEX);

 Serial.print(0x0F & (canMsg.b[sidl] >> 4), HEX);
  Serial.print(0x0F & canMsg.b[sidl], HEX);

   Serial.write("N");

  for (i = d0; i <= d7; i++)
  {
      Serial.print(0x0F & (canMsg.b[i] >> 4), HEX);
       Serial.print(0x0F & canMsg.b[i], HEX);

  }

  Serial.write(";");
  */


    for (i = 9; i < 14; i++)                                         
    {   
      if (i==9) 
      {
        Checksum  = canMsg.b[i];
      }
      else 
      {
        Checksum  ^= canMsg.b[i];
      }
    }
 


    Serial.write(0x02);                                              // STX (02h). You must send STX as a byte, only then STX is recognized as a command

  for (i = 9; i < 14; i++)
    {
       Serial.print(0x0F & (canMsg.b[i] >> 4), HEX);
       Serial.print(0x0F & canMsg.b[i], HEX);
     }
    Serial.print(0x0F & (Checksum >> 4), HEX);
    Serial.print(0x0F & Checksum, HEX);


  Serial.write(0x0D);       // CR
  Serial.write(0x0A);       // LF

   Serial.write(0x03);                                               // ETX (03h). You must send ETX as a byte, only then ETX is recognized as a command



}

 


