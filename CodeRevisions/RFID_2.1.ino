/***********************************************************************************************************************************
* Version 2.1
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
*             
*             MEMORY USAGE CONCERNS - Unstable behavior may result form unregulated RAM consumption
*             
*             
***********************************************************************************************************************************/



/***********************************************************************************************************************************
* LIBRARIES - Run Adafruit libraries (basically acty as SDKs
***********************************************************************************************************************************/
//#include <Adafruit_GFX.h>           // LCD Graphics Lib.
//#include <Wire.h>                   // I2C
//#include <SD.h>                     // Shield SD Reader
#include <SPI.h>                      // LCD Screen SPI
#include <Adafruit_PN532.h>           // RFID SHield
#include <Adafruit_ST7735.h>          // LCD lines etc
#include <Adafruit_seesaw.h>          // Button input managment
#include <Adafruit_TFTShield18.h>     // LCD Shield Lib




/***********************************************************************************************************************************
* PINOUT - Define the pins associated with the peripheral devices (e.g. RFID shield, LCD Shield)
***********************************************************************************************************************************/
//#define SD_CS       ( 4 )             // Chip select line for SD card on Shield
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
uint8_t state = 0;
uint8_t asciiNum = 65;
uint8_t cursorPos = 0;
uint8_t screenDelay = 100;
uint16_t buttonDelay = 350;
uint32_t buttons = 0; 

char asciiString[2] = {65};
char discName[20] = {};



/***********************************************************************************************************************************
* SETUP: This will run only once, before the main loop.  Set configurations in this function.
***********************************************************************************************************************************/
void setup() {

  configTFTShield();
  configRFIDShield();
  
}


/***********************************************************************************************************************************
* LOOP: This loop will run continuously.  Place additional functions below the end of the loop.
*       searchAndReadTag()        - Holds program in this function until a tag is found
*       awaitNextSearchCommand()  - Awaits sereall monitor input before searching for another tag
***********************************************************************************************************************************/
void loop(){

  switch(state){
    case 0:
      state_0();
      break;
      
    case 1:
      state_1();
      break;

    case 2:   
      state_2();
      break;

    case 3:
      state_3();
      break;

    case 4:
      state_4();
      break;

    default:   
      clearScreen();
      stateMachineError();
      while(1);  
      break;

  }
  
}



/*-------------------------------------------------  PROGRAM STATES --------------------------------------------------------------*/

/***********************************************************************************************************************************
* STATE 0: Display "Welcome Screen" until "continue" is pressed (btn 2)
************************************************************************************************************************************/
void state_0(){
  
  welcomeScreen();  
  delay(screenDelay);
  
  buttons = ss.readButtons();   
  while((buttons & TFTSHIELD_BUTTON_2)){   buttons = ss.readButtons();    }
  
  state = 1;

  return;
}


/***********************************************************************************************************************************
* STATE 1: Displays the "Home Screen".  The user will use the joystick and btn 2 to select an option.
************************************************************************************************************************************/
void state_1(){
  
  clearScreen();  
  homeScreen();   
  delay(screenDelay);

  uint8_t pressCount = 1;
  
  buttons = ss.readButtons();
  while((buttons & TFTSHIELD_BUTTON_2)){

    buttons = ss.readButtons(); 
    if( !(buttons & TFTSHIELD_BUTTON_UP) || !(buttons & TFTSHIELD_BUTTON_DOWN)){ 
        delay(buttonDelay);
        pressCount++;
        toggleHomeOptions(pressCount);
    }
  }

  pressCount % 2 == 1  ?  (state = 2)  :  (state = 3);
        
  return;
}


/***********************************************************************************************************************************
* STATE 2: Displays the "Search Screen" until a tag is found then display "Results Screen" until "Done" is selected.
************************************************************************************************************************************/
void state_2(){
  
  clearScreen();  
  searchScreen(); 
    
  String readResult = readData();
  
  clearScreen();  
  readResultsScreen(readResult);

  buttons = ss.readButtons(); ;
  while((buttons & TFTSHIELD_BUTTON_2)){   buttons = ss.readButtons();   }
      
  state = 1;

  return;
}


/***********************************************************************************************************************************
* STATE 3: Displays the "Edit Data Screen" where it uses helper functions to process the users inputs and character selection.
*          This state also controls the backend logic that stores the users entry to send to EEPROM.
************************************************************************************************************************************/
void state_3(){
  
  clearScreen();  
  editDataScreen();    
  delay(screenDelay);
  
  buttons = ss.readButtons(); ;
  while((buttons & TFTSHIELD_BUTTON_1)){   
    
    buttons = ss.readButtons();   
    if(!(buttons & TFTSHIELD_BUTTON_UP))      nextCharacter();
    if(!(buttons & TFTSHIELD_BUTTON_DOWN ))   prevCharacter();
    if(!(buttons & TFTSHIELD_BUTTON_IN   ))   shiftCharacter();
    if(!(buttons & TFTSHIELD_BUTTON_RIGHT))   selCharacter();
    if(!(buttons & TFTSHIELD_BUTTON_LEFT ))   delCharacter();
    if(!(buttons & TFTSHIELD_BUTTON_2    ))   spaceCharacter();
      
  }
  
  discName[cursorPos] = asciiNum;

  cursorPos = 0;
  state = 4;

  return;
}


/***********************************************************************************************************************************
* STATE 4: Displays the "Instructions Screen" and when a tag is found, the program will return the results of the write attempt.
************************************************************************************************************************************/
void state_4(){
  
  clearScreen();
  instructionScreen();
  delay(screenDelay);    

  String writeResult = writeData();
  delay(300);   

  clearScreen();
  writeResultsScreen(writeResult);

  buttons = ss.readButtons(); ;
  while((buttons & TFTSHIELD_BUTTON_2)){   buttons = ss.readButtons();   }
    
  state = 1;

  return;
}










/*-------------------------------------------------  DATA ENTRY HELPERS  ---------------------------------------------------------*/
void spaceCharacter(){
  
  delay(buttonDelay);

  discName[cursorPos] = asciiNum;
  
  cursorPos++;

  asciiNum = 32;
  discName[cursorPos] = asciiNum;

  cursorPos++;
  
  asciiNum = 65;
  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_WHITE , 30 + (6 * cursorPos), 70,  asciiString);

  return;
}

void delCharacter(){
  
  delay(buttonDelay);

  discName[cursorPos] = asciiNum;                                           // Store the current character in order to delete it           
  asciiString[0] = discName[cursorPos];                                     // Cast int to string
  dispMessage(1, ST77XX_BLACK , 30 + (6 * cursorPos), 70,  asciiString);    // Remove the character form the display
  discName[cursorPos] = 0;                                                  // Replace old index value with null
  
  cursorPos--;                                                              // Shift cursor on screen and in array 
  
  asciiNum = discName[cursorPos];                                           // Store the current character in order to delete it   
  asciiString[0] = asciiNum;                                                // Cast int to string    
  dispMessage(1, ST77XX_WHITE , 30 + (6 * cursorPos), 70,  asciiString);    // Remove the character form the display
  discName[cursorPos] = 0;                                                  // Replace old index value with null
  
  return;
  
}

void selCharacter(){
  
  delay(buttonDelay);
  
  discName[cursorPos] = asciiNum;
  
  cursorPos++;
  
  asciiNum = 65;
  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_WHITE , 30 + (6 * cursorPos), 70,  asciiString);

  return;
}

void nextCharacter(){

  delay(buttonDelay);
  
  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_BLACK ,30 + (6 * cursorPos), 70,  asciiString);
               
  asciiNum++;
        
  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_WHITE ,30 + (6 * cursorPos), 70,  asciiString);

  return;
}

void prevCharacter(){
  
  delay(buttonDelay);

  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_BLACK ,30 + (6 * cursorPos), 70,  asciiString);
               
  asciiNum--;
        
  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_WHITE ,30 + (6 * cursorPos), 70,  asciiString);

  return;
}

void shiftCharacter(){
  
  delay(buttonDelay);

  dispMessage(1, ST77XX_BLACK ,30 + (6 * cursorPos), 70,  asciiString);

  if     (asciiNum >= 65 && asciiNum <= 91 )  asciiNum = asciiNum + 32;  // If upper case go lower case
  else if(asciiNum >= 97 && asciiNum <= 122)  asciiNum = 48;             // If lower case go to numbers
  else if(asciiNum >= 48 && asciiNum <= 57 )  asciiNum = 33;             // If numbers go to special characters
  else    asciiNum = 65;

  asciiString[0] = asciiNum;
  dispMessage(1, ST77XX_WHITE ,30 + (6 * cursorPos), 70,  asciiString);

  return;
}










/*-------------------------------------------------  PROGRAM SCREENS  -------------------------------------------------------------*/
void welcomeScreen(){
  
  dispMessage(1, ST77XX_WHITE, 33,  57, "Disc Finders");
  dispMessage(1, ST77XX_RED,   110, 57, "PRO");

  delay(1000);
  dispMessage(1, ST77XX_WHITE, 19, 120, "Continue");
  
  return;
}

void homeScreen(){
  
  dispMessage(1, ST77XX_WHITE, 33,  0,   "Disc Finders");
  dispMessage(1, ST77XX_RED,   110, 0,   "PRO");

  dispMessage(1, ST77XX_GREEN, 33,  45,  "Search For Disc");
  dispMessage(1, ST77XX_WHITE, 33,  70,  "Change Disc Name");

  dispMessage(1, ST77XX_WHITE, 23,  120, "Select");

  return;
}

void toggleHomeOptions(uint8_t numPressed){
  
  if((numPressed % 2) == 0){
    dispMessage(1, ST77XX_WHITE, 33, 45,   "Search For Disc");
    dispMessage(1, ST77XX_GREEN, 33, 70,   "Change Disc Name");
  }
  
  else{
     dispMessage(1, ST77XX_GREEN, 33, 45,  "Search For Disc");
     dispMessage(1, ST77XX_WHITE, 33, 70,  "Change Disc Name");
   }
   
  return;
}

void searchScreen(){
  
  dispMessage(1, ST77XX_WHITE, 45, 57,   "Searching...");
  
  return;
}

void readResultsScreen(String discData){
  
  dispMessage(1, ST77XX_GREEN, 45, 57,   discData);
  dispMessage(1, ST77XX_WHITE, 26, 120,  "Done");

  return;
}

void editDataScreen(){
  
  dispMessage(1, ST77XX_RED,   30, 45,   "Enter Disc Name");    
  dispMessage(1, ST77XX_WHITE, 30, 70,   "A");
  dispMessage(1, ST77XX_RED,    0, 120,  "Save");
  dispMessage(1, ST77XX_WHITE, 29, 120,  "Space");

  return;
}

void instructionScreen(){
  
  dispMessage(1, ST77XX_WHITE, 32, 57,   "Place tag in the");
  dispMessage(1, ST77XX_WHITE, 8,  67,   "proximity of the device");

  return;
}

void storingDataScreen(){
  
  dispMessage(1, ST77XX_WHITE, 35, 57,   "Storing Data...");
  
  return; 
}

void writeResultsScreen(String storingResult){

    dispMessage(1, ST77XX_GREEN, 10, 57,  storingResult);
    dispMessage(1, ST77XX_WHITE, 26, 120, "Done");

    return;
}

void seesawErrorScreen(){
  
  dispMessage(1, ST77XX_WHITE,  36, 57,   "seeSaw Error!");
  dispMessage(1, ST77XX_WHITE,  8,  67,   "Please restart device");                        
    
  return;
}

void rfidErrorScreen(){
  
  dispMessage(1, ST77XX_WHITE,  10, 57,   "rfid shield Error!");
  dispMessage(1, ST77XX_WHITE,  8,  67,   "Please restart device");                        
    
  return;
}

void stateMachineError(){
    
    dispMessage(1, ST77XX_WHITE, 5, 57,   "State Machine Error");

    return;
}









/*-------------------------------------------------  EEProm MEMORY ACCESS  ---------------------------------------------------------*/

/***********************************************************************************************************************************
* WRITE TAG DATA: Waits for a tag to read -> Authenticates EEProm block to read from -> Reads and stores data -> displays data
************************************************************************************************************************************/
String writeData(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          // Buffer to store the returned UID
  uint8_t uidLength;                                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   // Waits here

  clearScreen();
  storingDataScreen();
  delay(100);  
  

  /*************** If its the correct card type ******************************************/
  if(success && uidLength == 4){
    
    success = 0;
    uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keyb);


      /********** If EEPROM has been authenticated, attempt to write ********************/
      if (success){
        
        success = 0;
        success = nfc.mifareclassic_WriteDataBlock (4, discName);

        
        /********* If data has been succeffully written, return message ********************/
        if(success) return "Complete!";
        else        return "Error writing data";
        
      }

      /*********** Authentication error occured and return error message ********************/
      else return "Authentication failed";                     
   
  }
  
  return "ERROR";
}


/***********************************************************************************************************************************
* READ TAG DATA: Waits for a tag to read -> Authenticates EEProm block to read from -> Reads and stores data -> displays data
* Notes:  Key value is used to authenticate EEPROM
*         Waits at readPassiveTargetID() until a tag is found
*         uid[]     - Buffer to store the returned UID
*         uidLength - Length of the UID (4 or 7 bytes depending on ISO14443A card type)
************************************************************************************************************************************/
String readData(){
  
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };          
  uint8_t uidLength;                               
  
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);   


  /*************** If its the correct card type ******************************************/
  if(success && uidLength == 4){                                
    
    success = 0;
    uint8_t keyb[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };   
    success = nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keyb);


      /*********** If data has been authenticated, attempt to read ************************/
      if(success){                                              
        
        success = 0;
        uint8_t rcvData[20];
        success = nfc.mifareclassic_ReadDataBlock(4, rcvData);        


        /********* If data has been succeffully retrieved, return data ********************/
        if(success)  return rcvData;                            
        else         return "EEPROM not read";                    
        
      }


      /*********** Authentication error occured and return error message ********************/
      else return "Authentication failed";                      // A problem occured while authenticating the EEPROM bank
      
  }
  
  return "ERROR";
}
      









/*-------------------------------------------------  LCD HELPER FUNCTIONS  --------------------------------------------------------*/

/***********************************************************************************************************************************
* CLEAR SCREEN: Sets all display pixels to black, effectively clearing the screen of any text.
************************************************************************************************************************************/
void clearScreen(){
  
    tft.fillScreen(ST77XX_BLACK);                           // Set screen to black
    
    return;
}


/***********************************************************************************************************************************
* NEW LCD MMESSAGE: Uses the sent peramiters to position and display text on the LCD.
************************************************************************************************************************************/
void dispMessage(uint8_t txtSize, uint16_t txtColor, uint8_t xCor, uint8_t yCor, String message){
    
  tft.setTextSize(txtSize);                                 // Set text size
  tft.setTextColor(txtColor);                               // Set text color
  tft.setCursor(xCor, yCor);                                // Set cursor on grid
  tft.print(message);                                       // Show message

  return;
}








      
/*-------------------------------------------------  CONFIGURATION FUNCTIONS --------------------------------------------------------*/

/***********************************************************************************************************************************
* CONFIGURE TFT SHIELD: Connect the LCD
************************************************************************************************************************************/
void configTFTShield(){

  enableSD_TFTPins();                               // start by enabling both SD and TFT pins on arduino
  startSeeSaw();                                    // Start seesaw helper chip
  initLCD();                                        // Initialize the LCD for operation

  return;
}

/***********************************************************************************************************************************
* CONFIGURE RFID: Begins the communication with the nfc.  Communication is verified through the serial monitor.
************************************************************************************************************************************/
void configRFIDShield(){
  
  nfc.begin();                                        // Begin the communication to the nfc shield                      

  uint32_t versiondata = nfc.getFirmwareVersion();    // Get the firmware version to check communication

  if (! versiondata){                                 // Frimware version was not collected

    rfidErrorScreen();                                // Notify us if the board is not found
    while (1);                                        // stop the program if not found
    
  }

  nfc.SAMConfig();                                    // configure the PN532 to read MiFare cards.
  
  return;
}








/*-------------------------------------------------  INITIALIZATION FUNCTIONS -------------------------------------------------------*/

/***********************************************************************************************************************************
 * CONFIGURE ARDUINO PINS - Configure the digital pins of the arduino
************************************************************************************************************************************/
void enableSD_TFTPins(){
  
  //pinMode(      SD_CS,  OUTPUT);                      // Set SD card chip select pin as output         
  //digitalWrite( SD_CS,  HIGH  );                      // Set SD card chip select pin as on
  
  pinMode(      TFT_CS, OUTPUT);                        // Set TFT chip select pin as output 
  digitalWrite( TFT_CS, HIGH  );                        // Set TFT card chip select pin as on
 
  return;
}

/***********************************************************************************************************************************
 * INITIALIZE SEESAW - Initialize the Seesaw chip for operation.
************************************************************************************************************************************/
void startSeeSaw(){
  
  if (!ss.begin()){                     
    
    seesawErrorScreen();                              // If there is a problem starting the seesaw chip, report it
    while(1);                                         // Prevent the program from running while the chip is inop
    
  }
  
  return;  
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

  return;
}
