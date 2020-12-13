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

IMPORTANT FYI for Expanding Current Program - The libraries that Adafruit have made for these shields are large. If you include all of them, you will experience instability due to RAM restrictions on the R3 Uno.

THE FIX - Remove the libraries you aren't using. I removed Wire.h , SD.h , and Adafruit_GFX.h

Be sure to allow enough space for local variables durring runtime.


<img align = "center" width="432"  height = "600" src = "https://user-images.githubusercontent.com/37875517/101992112-d874a100-3c7e-11eb-9840-a6306b19d096.JPG">



## Artifacts

* [Project Proposal]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/Proposal.md)

* [Research and Documentation]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/artifacts/Research%20and%20Documentation.pdf)

* [Program Library Details]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/artifacts/Program%20Library%20Details.pdf)

* [Arduino Library Install Guide]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/artifacts/RFID%20Shield%20Library%20Install%20Directions.pdf)

* [Design Proposals 1]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/artifacts/Design%20Proposal%201.pdf)

* [Project Timeline](https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/Gantt%20Chart.pdf)

* [Assigned Tasks Overview]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/Tasks.md)

* [SRS Midterm]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/software_requirements_specification.md )

* [Use Case Diagrams]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/tree/master/artifacts )

* [Midterm Presentation]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/Midterm%20Presentation.pdf )

* [State Machine Final]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/state_machine_final.pdf )

* [Final Presentation](https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/blob/master/docs/DiscFinders%20Final%20Presentation.pdf)

* [Code Revisions]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/tree/master/CodeRevisions)

* [Meeting Minutes]( https://github.com/MouraCMichael/GVSU-CIS350-DiscFinders/tree/master/meetings )
