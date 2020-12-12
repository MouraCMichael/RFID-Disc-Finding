# Disc Finders

All disc golfers will eventually throw a bad shot on the course.  Sometimes they are so bad, it can be difficult to find where the disc landed.  This project is dedicated to helping speed up the search process, and prevent players from losing discs.  Our goal is to write an arduino program which impliments RFID technology to accomplish this task.

## Team Members and Roles

* [Timothy Beler](https://github.com/at-a-minimum1/CIS350-HW2-BELER)
* [Corey Moura](https://github.com/MouraCMichael/CIS350-HW2-Moura.git)

## Required Hardware

* [Arduino R3](https://www.sparkfun.com/products/11021)
* [PN532 RFID Shield](https://www.adafruit.com/product/789)
* [Interactive Display Shield](https://www.adafruit.com/product/802)
* [Shield Header Pins](https://www.adafruit.com/product/85)
* [90* Double Stack Header](https://www.sparkfun.com/products/12792)
* [Female Jumper Wires](https://www.sparkfun.com/products/12796)
* [Powerboost 500 (Optional)](https://www.adafruit.com/product/2078)
* [Lithium Battery (Optional)](https://www.adafruit.com/product/2011)

## Run Instructions

1. Purchase above hardware components
2. Solder shield header pins to the RFID sheild (important for stacking the other shields)
3. Solder the provided header pins to the LCD shield
4. Solder the 90 degree header pins to the LCD sheild
5. Replace the 2x3 header pins for SPI communication with the LCD for the 90 degree header pins on the Arduino R3 (see pictures)
6. Join the 90 degree Arduino header piins to the 90 degree header pins on the LCD shield via jumper wires
7. Place the RFID shield to the Arduino and upload the [RFID tag formatting program](https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/CodeRevisions/_formatNDEF.ino)
8. Follow the instructions via serial monitor durring the formatting process.
9. Attach the LCD shield and upload the [latest program version](https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/CodeRevisions/RFID_2.1.ino)
10.Run the program and test that the features work correctly.



<img align = "center" width="432"  height = "600" src = "https://user-images.githubusercontent.com/37875517/101992112-d874a100-3c7e-11eb-9840-a6306b19d096.JPG">
