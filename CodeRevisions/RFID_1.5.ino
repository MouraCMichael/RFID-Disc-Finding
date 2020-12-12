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

#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_seesaw.h>
#include <Adafruit_TFTShield18.h>


/***********************************************************************************************************************************
* PINOUT: PN532_IRQ   - Digital I/O Interrupt pin for I2C shield
*         PN532_RESET - Not connected by default on the NFC Shield
*         Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET) - Define nfc object for the shield with an I2C connection:
***********************************************************************************************************************************/
#define PN532_IRQ   (2)                       
#define PN532_RESET (3)                      

#define SD_CS    4  // Chip select line for SD card on Shield
#define TFT_CS  10  // Chip select line for TFT display on Shield
#define TFT_DC   8  // Data/command line for TFT on Shield
#define TFT_RST  -1  // Reset line for TFT is handled by seesaw!


Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET); 
Adafruit_TFTShield18 ss;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


uint8_t buttonhistory = 0;

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
  while (!Serial);

  connectAndConfigLCD();
  connectAndConfigRFID();
  
}


/***********************************************************************************************************************************
* LOOP: This loop will run continuously.  Place additional functions below the end of the loop.
*       searchAndReadTag()        - Holds program in this function until a tag is found
*       awaitNextSearchCommand()  - Awaits sereall monitor input before searching for another tag
***********************************************************************************************************************************/
void loop(){
  
  uint32_t buttons = ss.readButtons();
  tft.setTextSize(3);

  if(! (buttons & TFTSHIELD_BUTTON_1)){
    tft.setTextColor(ST77XX_BLUE);
    tft.setCursor(0, 140);
    tft.print("1");
    buttonhistory |= 16;
    
    searchAndShowEmbeddedID();
    
  }
  
  
  //delay(1000);

  
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
* CONNECT AND CONFIGURE: Begins the communication with the nfc.  Communication is verified through the serial monitor.
************************************************************************************************************************************/
void connectAndConfigLCD(){

    // start by disabling both SD and TFT
  pinMode(TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  // Start seesaw helper chip
  if (!ss.begin()){
    Serial.println("seesaw could not be initialized!");
    while(1);
  }
  Serial.println("seesaw started");
  Serial.print("Version: "); Serial.println(ss.getVersion(), HEX);

  // Start set the backlight off
  ss.setBacklight(TFTSHIELD_BACKLIGHT_OFF);
  // Reset the TFT
  ss.tftReset();

  // Initialize 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

  Serial.println("TFT OK!");
  tft.fillScreen(ST77XX_CYAN);

//  Serial.print("Initializing SD card...");
//  if (!SD.begin(SD_CS)) {
//    Serial.println("failed!");
//  } else {
//    Serial.println("OK!");
//    File root = SD.open("/");
//    printDirectory(root, 0);
//    root.close();
//    bmpDraw("/parrot.bmp", 0, 0);
//  }

  // Set backlight on fully
  // ss.setBacklight(TFTSHIELD_BACKLIGHT_ON);
  // Or you can set the backlight one third on
  // ss.setBacklight(TFTSHIELD_BACKLIGHT_ON / 3);
  // Or dim it up
  for (int32_t i=TFTSHIELD_BACKLIGHT_OFF; i<TFTSHIELD_BACKLIGHT_ON; i+=100) {
    ss.setBacklight(i);
    delay(1);
  }
  delay(100);
  tft.fillScreen(ST77XX_RED);
  delay(100);
  tft.fillScreen(ST77XX_GREEN);
  delay(100);
  tft.fillScreen(ST77XX_BLUE);
  delay(100);
  tft.fillScreen(ST77XX_BLACK);
 
  tft.setTextSize(1);
  tft.setTextColor(ST77XX_WHITE);
  tft.setCursor(0, 0);
  tft.print("DISC FINDERS!!!");



 uint8_t buttonhistory = 0;

}





/***********************************************************************************************************************************
* SEARCH AND SHOW EMBEDDED ID #:  Awaits a signal back from an RFID tag.  Once found, the tags embedded ID number is displayed.
*       nfc.readPassiveTargetID - aits for a tag, when one is found 'uid' will be populated and its array length
***********************************************************************************************************************************/
void searchAndShowEmbeddedID(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          // Buffer to store the returned UID
  uint8_t uidLength;                                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    
  Serial.println("Searching for Tag...\n");

  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits here
   
  if(success){
    
    Serial.print("\tEmbeded ID #: ");
    
    for(int i = 0; i < uidLength; i++)  Serial.print(uid[i]);

    Serial.println();
    
    success = 0;
    

    
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
          Serial.println("\n\tOoops ... unable to read the requested block.  Try another key?");
        }
      }
      else
      {
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
    }
      
  }


  
  
  Serial.println();
  
}
