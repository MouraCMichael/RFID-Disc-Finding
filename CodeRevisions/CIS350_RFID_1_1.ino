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



/***********************************************************************************************************************************
* SETUP: This will run only once, before the main loop.  Set configurations in this function.
*        
*        Serial.begin(9600) - Serial communication rate. Must change in serial monitor and here if another value is desired
*        connectAndConfigRFID() - 
***********************************************************************************************************************************/
void setup() {

  Serial.begin(9600);                                  

  connectAndConfigRFID();
  
  Serial.println("Waiting for an ISO14443A Card ...\n");    // Shows GUI message in console

}


/***********************************************************************************************************************************
* LOOP: This loop will run continuously.  Place additional functions below the end of the loop.
*       searchAndReadTag()        - Holds program in this function until a tag is found
*       awaitNextSearchCommand()  - Awaits sereall monitor input before searching for another tag
***********************************************************************************************************************************/
void loop(){
  
  searchAndReadTag();
  awaitNextSearchCommand();

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
* SEARCH AND SHOW TAG ID: 
***********************************************************************************************************************************/
void searchAndReadTag(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
    
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits for a tag, when one is found 'uid' will be populated and its array length
  
  Serial.println("Check");
  
  if(success){
    
    Serial.print("\tFound tag: ");
    
    for(int i = 0; i < uidLength; i++)  Serial.print(uid[i]);
      
  }
  
}



/***********************************************************************************************************************************
 * 
 ***********************************************************************************************************************************/
void awaitNextSearchCommand(){
      
  Serial.println("\n\nSend a character to scan another tag!");       // Wait a bit before trying again
  Serial.flush();

  while (!Serial.available());
    
  while (Serial.available()) {
    Serial.read();
  }
  
  Serial.print("\n");       // Wait a bit before trying again
  Serial.flush();   
  delay(1000);

}
