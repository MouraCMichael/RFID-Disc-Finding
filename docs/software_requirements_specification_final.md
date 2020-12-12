# Overview 
< The purpose of this document is to outline the features of the DiscFinders Application. The document provides the scope of the project for the developers and outlines the framework for its development. This document is intended for project team members and the instructor in CIS 350 at GVSU.>
 
# Software Requirements
<This section contains both the functional and non-functional requirements of this project. Each requirement is located under a specific category in either the functional of non-functional heading to which it applies.>  

## Functional Requirements 

### RFID Communication 
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| FR1  |  The device shall transmit data via RFID signal at 13.56Mhz            | TC1 | 
| FR2  |  The device shall receive data via RFID signal of 13.56Mhz             | TC2 | 
| FR3  |  The device shall communicate to all NFC Data Exchange Formatted tags  | TC3 | 
| FR4  |  The device shall communicate with multiple NDEF tags                  | TC3 | 
| FR5  |  The device shall communicate up to 10cm                               | TC3 | 

### Display
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| FR6  |  The device shall display the tag name for every successfully read attempt          | TC1 |
| FR7  |  The device shall display an error message for every unsuccessful read attempt      | TBD |
| FR8  |  The device shall display a message when data has been successfully written to tag  | TC1 |
| FR9  |  The device shall display an error message for every unsuccessful write to tag      | TC1 |
| FR10 |  The display shall display when it is actively searching for the tag                | TC1 |

### Interactive Switches
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| FR11 |  A switch shall power on and power off the device                               | TC1 |
| FR12 |  A switch shall allow the user to toggle between reading and writing to a tag   | TBD |
| FR13 |  A switch shall initiate the reading process                                    | TC1 |
| FR14 |  A switch shall initiate the writing process                                    | TC1 |
| FR15 |  A switch shall initiate a soft reset of the device                             | TC1 |  

<br/><br/>

## Non-Functional Requirements 

### RFID Communication 
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| NFR1 | The device shall reliably read data from tag          | TC4 | 
| NFR2 | The device shall reliably write data to tag           | TBD | 
| NFR3 | The device shall easily switch between RFID signals   | TC6 | 
| NFR4 | The device shall not transmit with audible noise      | TC6 | 
| NFR5 | The device shall easily pair with the RFID tag        | TC6 | 

### Display 
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| NFR1 |  The display shall be easy to read                                                         | TC4 | 
| NFR2 |  The display shall noticeably change when near the device                                  | TBD | 
| NFR3 |  The device shall provide constant feedback to the user about the RFID tag when searching  | TC6 | 
| NFR4 |  The device shall provide accurate character feedback prior to writing data                | TC6 | 
| NFR5 |  The device shall have intuitive screen layouts                                            | TC6 | 


### Interactive Switches 
| ID | Requirement | Test Cases |
| :-------------: | :----------: | :----------: | 
| NFR1 |  The power switch shall be in a location easy to press       | TC4 | 
| NFR2 |  The soft reset switch shall be in a location easy to press  | TBD | 
| NFR3 |  The switches shall not interfere with holding the device    | TC6 | 
| NFR4 |  The switches must not interfere with reading the display    | TC6 | 
| NFR5 |  The switch layout must be intuitive                         | TC6 | 


# Test Specification
<Description of what this section is>
## Unit tests
(copy/paste the below table a minimum of 10 times) 
| ID | Description | Steps | Input Values | Expected Output | Actual Output | Pass/Fail | Requirement Link |
| :-------------: | :----------: | :----------: | :----------: | :----------: | :----------: | :----------: | :----------: | 
| TC1 | <TC1 description> | <steps to execute TC1> | <input values to this test case> | <expected output as a result of test case> | <actual output of test case> | <did it pass or fail?> | <requirement IDs this test case is linked to> | 
## Integration tests
(copy/paste the above table a minimum of 5 times) ## System tests
(copy/paste the above table a minimum of 5 times) # Software Artifacts
<Describe the purpose of this section>
* [I am a link](to_some_file.pdf) 






