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
*             LIBRARIES:  Wire.h            - https://www.arduino.cc/en/Reference/Wire  (I2C Communications Library)
*                         Adafruit_PN532.h  - https://cdn-learn.adafruit.com/downloads/pdf/adafruit-pn532-rfid-nfc.pdf
*                         Adafruit_GFX.h    - https://github.com/adafruit/Adafruit-GFX-Library/blob/master/Adafruit_GFX.h
*                         Adafruit_ST7735.h - https://github.com/adafruit/Adafruit-ST7735-Library/blob/master/Adafruit_ST7735.h
*                         
*                         
*             Some ready-made 16-bit ('565') color settings:  ST77XX_BLACK  ||  ST77XX_WHITE  ||  ST77XX_RED      ||  ST77XX_GREEN 
*                                                             ST77XX_BLUE   ||  ST77XX_CYAN   ||  ST77XX_MAGENTA  ||  ST77XX_YELLOW
*                                                             ST77XX_ORANGE
*
***********************************************************************************************************************************/



/***********************************************************************************************************************************
* LIBRARIES - Run Adafruit libraries (basically acty as SDKs
***********************************************************************************************************************************/
#include <SD.h>                       // Shield SD Reader
#include <SPI.h>                      // LCD Screen SPI
#include <Wire.h>                     // I2C
#include <Adafruit_PN532.h>           // RFID SHield
#include <Adafruit_GFX.h>             // LCD Graphics Lib.
#include <Adafruit_ST7735.h>          // LCD lines etc
#include <Adafruit_seesaw.h>          // Button input managment
#include <Adafruit_TFTShield18.h>     // LCD Shield Lib



/***********************************************************************************************************************************
* PINOUT - Define the pins associated with the peripheral devices (e.g. RFID shield, LCD Shield)
***********************************************************************************************************************************/
#define SD_CS       ( 4 )             // Chip select line for SD card on Shield
#define TFT_CS      ( 10)             // Chip select line for TFT display on Shield
#define TFT_DC      ( 8 )             // Data/command line for TFT on Shield
#define TFT_RST     (-1 )             // Reset line for TFT is handled by seesaw
#define PN532_IRQ   ( 2 )             // Digital I/O Interrupt pin for I2C shield          
#define PN532_RESET ( 3 )             // Not connected by default on the NFC Shield        



/***********************************************************************************************************************************
* PROGRAM OBJECTS - Create the objects used to access functions in the libraries
***********************************************************************************************************************************/
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET); 
Adafruit_TFTShield18 ss;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);



/***********************************************************************************************************************************
* GLOBAL VARIABLES - Global variables are placed in this section.
***********************************************************************************************************************************/
uint8_t buttonhistory = 0;


/***********************************************************************************************************************************
* SETUP: This will run only once, before the main loop.  Set configurations in this function.
***********************************************************************************************************************************/
void setup() {

  Serial.begin(115200);                                  
  while (!Serial);

  configTFTShield();
  configRFIDShield();

  welcomeScreen();

}


/***********************************************************************************************************************************
* LOOP: This loop will run continuously.  Place additional functions below the end of the loop.
*       searchAndReadTag()        - Holds program in this function until a tag is found
*       awaitNextSearchCommand()  - Awaits sereall monitor input before searching for another tag
***********************************************************************************************************************************/
void loop(){

  uint32_t buttons = ss.readButtons();

  if(! (buttons & TFTSHIELD_BUTTON_1)){
    tft.setTextColor(ST77XX_BLUE);
    tft.setCursor(0, 140);
    tft.print("1");
    buttonhistory |= 16;
    
    //searchAndShowEmbeddedID();
    readData();
  }

  if(! (buttons & TFTSHIELD_BUTTON_2)){
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(50, 140);
    tft.print("2");
    buttonhistory |= 32;

    writeData();
  }

  if(! (buttons & TFTSHIELD_BUTTON_3)){
    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(100, 140);
    tft.print("3");
    buttonhistory |= 64;
    
  }
 
}



///*-------------------------------------------------  PROGRAM SCREENS  -------------------------------------------------------------*/
void welcomeScreen(){
  dispMessage(1, ST77XX_WHITE , 33, 57, "Disc Finders");
  dispMessage(1, ST77XX_RED , 33 + 77, 57, "PRO");

  delay(1500);
  dispMessage(1.5, ST77XX_WHITE , 19, 120, "Continue");
}














/*-------------------------------------------------  EEProm MEMORY ACCESS  ---------------------------------------------------------*/


/***********************************************************************************************************************************
* WRITE TAG DATA: Waits for a tag to read -> Authenticates EEProm block to read from -> Reads and stores data -> displays data
************************************************************************************************************************************/
void writeData(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          // Buffer to store the returned UID
  uint8_t uidLength;                                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits here


  /*************** If't has found a tag to communicate with ********************/
  if(success){
    
     success = 0;


    /*************** If its the correct card type ********************/
    if (uidLength == 4){        
      
      Serial.println("\tAuthenticating block 4 in EEPROM");
      uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keyb);


      /*************** If the block has been authenticated ********************/
      if (success){
        
        success = 0;
        Serial.println("\tBlocks 4-7 have been authenticated");
        uint8_t sendData[] = "Squirtle";
        success = nfc.mifareclassic_WriteDataBlock (4, sendData);
        
      }

    }

    /*************** Authentication failed ********************/   
    else{
        Serial.println("Ooops ... authentication failed: Try another key?");
    }
  }
}


/***********************************************************************************************************************************
* READ TAG DATA: Waits for a tag to read -> Authenticates EEProm block to read from -> Reads and stores data -> displays data
************************************************************************************************************************************/
void readData(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          // Buffer to store the returned UID
  uint8_t uidLength;                                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

  Serial.println("Searching for Tag...\n");
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits here

 /*************** If't has found a tag to communicate with ********************/
  if(success){
    
    success = 0;
    
    Serial.print("\tEmbeded ID #: ");
    for(int i = 0; i < uidLength; i++)  Serial.print(uid[i]);
    Serial.println(); 
    

    /*************** If its the correct card type ********************/
    if(uidLength == 4){
      
      Serial.println("\tAuthenticating block 4 in EEPROM");
      uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
      success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keyb);

      
      /*************** If the block has been authenticated ********************/
      if(success){
        
        success = 0;
        uint8_t rcvData[20];
        success = nfc.mifareclassic_ReadDataBlock(4, rcvData);        


        /*************** Data seems to have been read ********************/
        if (success){
          
          success = 0;
          Serial.println("\tReading Block 4...\n\t");                  

          for(int i = 0; i <= 15; i++){            
            char thisChar = rcvData[i];
            Serial.print(thisChar);
          }
   
          //Serial.println("");
          Serial.println(ST77XX_WHITE);

          
        }
        
        else{
          Serial.println("\n\tOoops ... unable to read the requested block.  Try another key?");
        }
      }
      
      else{
        Serial.println("Ooops ... authentication failed: Try another key?");
      }
      
    }
  }
}
      





/*-------------------------------------------------  LCD MESSAGE FUNCTIONS  --------------------------------------------------------*/

/***********************************************************************************************************************************
* CLEAR SCREEN: Sets all display pixels to black.
************************************************************************************************************************************/
void clearScreen(){
    tft.fillScreen(ST77XX_BLACK);                             // Set screen to black
}


/***********************************************************************************************************************************
* NEW LCD MMESSAGE: Uses the sent peramiters to position and display text on the LCD.
************************************************************************************************************************************/
void dispMessage(uint8_t txtSize, uint16_t txtColor, uint8_t xCor, uint8_t yCor, String message){
    
  tft.setTextSize(txtSize);                                 // Set text size
  tft.setTextColor(txtColor);                               // Set text color
  tft.setCursor(xCor, yCor);                                // Set cursor on grid
  tft.print(message);                                       // Show message
  
}





      
/*-------------------------------------------------  CONFIGURATION FUNCTIONS --------------------------------------------------------*/

/***********************************************************************************************************************************
* CONFIGURE TFT SHIELD: Connect the LCD
************************************************************************************************************************************/
void configTFTShield(){

  disableSD_TFT();                                  // start by disabling both SD and TFT
  startSeeSaw();                                    // Start seesaw helper chip
  initLCD();                                        // Initialize the LCD for operation

  uint8_t buttonhistory = 0;

  Serial.println("TFT  Shieled Configuration Complete");

}

/***********************************************************************************************************************************
* CONFIGURE RFID: Begins the communication with the nfc.  Communication is verified through the serial monitor.
************************************************************************************************************************************/
void configRFIDShield(){
  
  nfc.begin();                                        // Begin the communication to the nfc shield                      

  uint32_t versiondata = nfc.getFirmwareVersion();    // Get the firmware version to check communication

  if (! versiondata){                                 // Frimware version was not collected
    Serial.print("Didn't find PN53x board");          // Notify us if the board is not found
    Serial.print("Please Reset Device");              // Notify us if the board is not found

    while (1);                                        // stop the program if not found
  }

  //Serial.print("Chip Found: PN5"); 
  //Serial.println((versiondata>>24) & 0xFF, HEX);      // Got ok data, print it out!

  nfc.SAMConfig();                                    // configure the PN532 to read MiFare cards.
  
  Serial.println("RFID Shieled Configuration Complete");
  
}






/*-------------------------------------------------  INITIALIZATION FUNCTIONS -------------------------------------------------------*/

/***********************************************************************************************************************************
 * DISABLE TFT and SD - Disable these components while the initialization process is preformed.
************************************************************************************************************************************/
void disableSD_TFT(){
  
  pinMode(     SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  
  pinMode(     TFT_CS, OUTPUT);
  digitalWrite(TFT_CS, HIGH);
  
}

/***********************************************************************************************************************************
 * INITIALIZE SEESAW - Initialize the Seesaw chip for operation.
************************************************************************************************************************************/
void startSeeSaw(){
  
  if (!ss.begin()){
    Serial.println("seesaw could not be initialized!");
    while(1);
  }
  Serial.println("SeeSaw initialization Complete");
  //Serial.print("Version: "); Serial.println(ss.getVersion(), HEX);
  
}

/***********************************************************************************************************************************
 * INITIALIZE LCD - Initialize the LCD component on the TFT shield and set the initial screen configuration.
************************************************************************************************************************************/
void initLCD(){
   
  ss.setBacklight(TFTSHIELD_BACKLIGHT_OFF);           // Start set the backlight off
  ss.tftReset();                                      // Reset the TFT
  tft.initR(INITR_BLACKTAB);                          // initialize a ST7735S chip, black tab

  
  tft.fillScreen(ST77XX_BLACK);                       // Set screen to black
  ss.setBacklight(TFTSHIELD_BACKLIGHT_ON);            // Turn on backlight
  
  tft.setRotation(1);                                 // Rotate text to landscape

  tft.setTextSize(1);                                 // Set text size
  tft.setTextColor(ST77XX_WHITE);                     // Set text color
  tft.setCursor(0, 0);                                // Set cursor on grid

  Serial.println("LCD initalization Complete");
}
