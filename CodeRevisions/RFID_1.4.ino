/***********************************************************************************************************************************
* Version 1.1
* 
* Author 1:  Timothy Beler
* Author 2:  Corey Moura
* 
* Course:    CIS350
* 
* Professor: Fredericks
* 
* Description:     
*
* Notes:    Available RFID Shield Library Functions
*             
*             firmware_version    - Getthelatestfirmwareversion.
*             SAM_configuration   - configure the PN532 to read MiFare cards.
*             read_passive_target - Wait for a MiFare card to be available and return its UID when found.
*             call_function       - Send specified command to the PN532 and expect up to response_length bytes back in a response.
*             ntag2xx_read_block  - Read a block of data from the card.
*             ntag2xx_write_block - Writeablockofdatatothecard.
*             mifare_classic_write_block  -  Writeablockofdatatothecard.
*             mifare_classic_authenticate_block - AuthenticatespecifiedblocknumberforaMiFareclassiccard. mifare_classic_read_block -Readablockofdatafromthecard.
*
*             GitHub Examples:  https://github.com/adafruit/Adafruit-PN532
*
***********************************************************************************************************************************/



/***********************************************************************************************************************************
* LIBRARIES: Wire.h           - https://www.arduino.cc/en/Reference/Wire  (I2C Communications Library)
*            Adafruit_PN532.h - https://cdn-learn.adafruit.com/downloads/pdf/adafruit-pn532-rfid-nfc.pdf
***********************************************************************************************************************************/
#include <Wire.h>
#include <Adafruit_PN532.h>           



/***********************************************************************************************************************************
* PINOUT: PN532_IRQ   - Digital I/O Interrupt pin for I2C shield
*         PN532_RESET - Not connected by default on the NFC Shield
*         Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET) - Define nfc object for the shield with an I2C connection:
***********************************************************************************************************************************/
#define PN532_IRQ   (2)                       
#define PN532_RESET (3)                      

Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET); 



uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          // Buffer to store the returned UID
uint8_t uidLength;                                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)



/***********************************************************************************************************************************
* SETUP: This will run only once, before the main loop.  Set configurations in this function.
*        
*        Serial.begin(9600) - Serial communication rate. Must change in serial monitor and here if another value is desired
*        connectAndConfigRFID() - 
*        
*        https://circuitpython.readthedocs.io/projects/pn532/en/latest/api.html#adafruit-pn532-i2c
***********************************************************************************************************************************/
void setup() {

  Serial.begin(115200);                                  

  connectAndConfigRFID();

}


/***********************************************************************************************************************************
* LOOP: This loop will run continuously.  Place additional functions below the end of the loop.
*       searchAndReadTag()        - Holds program in this function until a tag is found
*       awaitNextSearchCommand()  - Awaits sereall monitor input before searching for another tag
***********************************************************************************************************************************/
void loop(){


  success = searchForTag(uid);
  
  if(success){
    showEmbeddedID();
  }
  //searchAndShowEmbeddedID();
  delay(1000);

}


/***********************************************************************************************************************************
* CONNECT AND CONFIGURE: Begins the communication with the nfc.  Communication is verified through the serial monitor.
************************************************************************************************************************************/
void connectAndConfigRFID(){
  
  nfc.begin();                                        // Begin the communication to the nfc shield                      

  uint32_t versiondata = nfc.getFirmwareVersion();    // Get the firmware version to check communication

  if (! versiondata){                                 // Frimware version was not collected
    Serial.print("Didn't find PN53x board");          // Notify us if the board is not found
    while (1);                                        // stop the program if not found
  }

  Serial.print("Chip Found: PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);   // Got ok data, print it out!

  nfc.SAMConfig();                                    // configure the PN532 to read MiFare cards.
    
}



/***********************************************************************************************************************************
* SEARCH AND SHOW EMBEDDED ID #:  Awaits a signal back from an RFID tag.  Once found, the tags embedded ID number is displayed.
*       nfc.readPassiveTargetID - aits for a tag, when one is found 'uid' will be populated and its array length
***********************************************************************************************************************************/
uint8_t searchForTag(uint8_t uid){

  Serial.println("Searching for Tag...\n");

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits here

  Serial.println(success);
  Serial.println(uidLength);
  Serial.println(uid);

  //for(int i = 0; i < uidLength; i++)  Serial.print(uid[i]);
  
  return success;
  
}



/***********************************************************************************************************************************
* SEARCH AND SHOW EMBEDDED ID #:  Awaits a signal back from an RFID tag.  Once found, the tags embedded ID number is displayed.
*       nfc.readPassiveTargetID - aits for a tag, when one is found 'uid' will be populated and its array length
***********************************************************************************************************************************/
void showEmbeddedID(){

 
    Serial.print("\tEmbeded ID #: ");
    
    for(int i = 0; i < uidLength; i++)  Serial.print(uid[i]);

    Serial.println();
    
    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ... 
      //Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    
      // Now we need to try to authenticate it for read/write access
      // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      Serial.println("\tAuthenticating block 4 in EEPROM");
      //uint8_t keya[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
      //uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
      uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
       //success = 0;
    // Start with block 4 (the first block of sector 1) since sector 0
    // contains the manufacturer data and it's probably better just
    // to leave it alone unless you know what you're doing
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keyb);
    
      if (success)
      {
        Serial.println("\tBlocks 4-7 have been authenticated");
        
        uint8_t sendData[] = "Innova FireBird";
    
        // If you want to write something to block 4 to test with, uncomment
        success = nfc.mifareclassic_WriteDataBlock (4, sendData);

        uint8_t rcvData[20];
        success = 0;
        
        // Try to read the contents of block 4
        success = nfc.mifareclassic_ReadDataBlock(4, rcvData);
    
        if (success)
        {
          // Data seems to have been read ... spit it out
          Serial.println("\tReading Block 4...");
          
          Serial.print("\n\t");
                    
          for(int i = 0; i <= 15; i++){            
            char thisChar = rcvData[i];
            
            Serial.print(thisChar);
            
          }
          
          Serial.println("");
      
          // Wait a bit before reading the card again
          delay(1000);
        }
        else
        {
          Serial.println("Ooops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
      
}


  
