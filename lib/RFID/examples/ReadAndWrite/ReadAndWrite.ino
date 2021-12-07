/*
 Read & Write
 Read the serial number of the IC card.
 
 < http://cod-box.net>
 */

#include <SPI.h>
#include <RFID.h>


RFID rfid(10,9);   


unsigned char serNum[5];
unsigned char status;
unsigned char str[MAX_LEN];
unsigned char blockAddr;        


unsigned char writeDate[16] ={
  'I', 'c', 'i', 'V', 'o', 't', 'r', ' e', 'C', 'h', 'a', 'i', 'n', 'e', '*', 0}; // Remplacer par votre text a l'ecrire sur la carte a puce.

unsigned char sectorKeyA[16][16] = {
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,
  {  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF  } ,};

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
}

void loop()
{
  rfid.findCard(PICC_REQIDL, str);
  
    if (rfid.anticoll(str) == MI_OK){      
      Serial.print("Numero de serie: ");
    
      for(int i = 0; i < 4; i++){
        Serial.print(0x0F & (str[i] >> 4),HEX);
        Serial.print(0x0F & str[i],HEX);
      }
      Serial.println("");
      memcpy(rfid.serNum,str,5);
    }
 
  rfid.selectTag(rfid.serNum);

  writeCard(4); // ecrire sur la carte
  readCard(4); // lire sur la carte
  rfid.halt(); // pause
} 

void writeCard(int blockAddr){
  
  if (rfid.auth(PICC_AUTHENT1A, blockAddr, sectorKeyA[blockAddr/4], rfid.serNum) == MI_OK)  //认证
  {  
    Serial.print("set the new card password, and can modify the data of the Sector: ");
    Serial.println(blockAddr/4,DEC);
    if(rfid.write(blockAddr, writeDate) == MI_OK){
      Serial.println("Ecriture sur la carte effectué avec succès !");
    }
  }
}
void readCard(int blockAddr){
  if ( rfid.auth(PICC_AUTHENT1A, blockAddr, sectorKeyA[blockAddr/4], rfid.serNum) == MI_OK)  //认证
  {
    Serial.print("Read from the blockAddr of the card : ");
    Serial.println(blockAddr,DEC);
    if( rfid.read(blockAddr, str) == MI_OK){
      Serial.print("The data is : ");
      Serial.println((char *)str);
    }
  }
}
