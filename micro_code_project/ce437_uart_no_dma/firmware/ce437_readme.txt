            Readme File for Code Example:
               CExxx -- UART no DMA
    ---------------------------------------------------

This file contains the following sections:
1. Code Example Description
2. Folder Contents
3. Suggested Development Resources
4. Reconfiguring the project for a different dsPIC33E device
5. Revision History


1. Code Example Description:
----------------------------
UART is configured with 9600 baud rate, 8 bit data, no parity bit and single stop bit.
A single letter will be transmitted upon each keypress on Explorer16 board.

2. Folder Contents:
-------------------
a. firmware
        This folder contains all the C, Assembler source files and include files(*.c,
        *.s, *.h) and project specific files used in demonstrating the described example. 
b. system_config
		This folder contains the chipset specific configuration code. More specifically it inturn contains a folder called exp16/ 
		which holds configuration files.
c. exp16/
		This folder contains various folders like dspic33ep512gm710/dspic33ep512mu810/dspic33ep256gp506 depending on the platform.Each platform folder contain,configuration 
		specific source files.

3. Suggested Development Resources:
-----------------------------------
        a. Explorer 16 Demo board with dspic33ep512gm710/dspic33ep512mu810/dspic33ep256gp506 controller

Note :- The PPS configuration in the source files changes with the device being used. The user is advised
to refer the datasheet and use the appropriate values for RPINR/RPOR registers for proper operation.

4. Reconfiguring the project for a different dsPIC33E device:
-------------------------------------------------------------
The Project/Workspace can be easily reconfigured for dspic33ep512gm710/dspic33ep512mu810 device.
Please use the following general guidelines:
        a. Change device selection within MPLAB® IDE to dspic33ep512gm710/dspic33ep512mu810 device of
        your choice by using the following menu option:
        MPLAB X>>Configuration drop-down option>><Listed Device Configuration>

        b. Re-build the MPLAB® project using the menu option:
        MPLAB X>>Build Main Project

        c. Download the hex file into the device and run.

5. Revision History :
---------------------
	      02/12/2014 - Code Example updated for dspic33ep512gm710/dspic33ep512mu810/dspic33ep256gp506

**********************************************************************/
* ADDITIONAL NOTES: 
*
* Peripheral Library was not utilized, Bit Addressing was used in order
* to show all details involved in initialization of UART.
*
**********************************************************************/
