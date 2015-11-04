#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "app.h"

#define TRUE 1
#define FALSE 0
#define ONE_VOLT 310
#define ONE_TENTH_VOLT 31
#define ONE_HUNDREDTH_VOLT 3


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************

void SYS_Initialize ( void ) ;
char readKeyboard(void);


APP_DATA appData = {
                    .messageLine1 = "Explorer 16 Demo" ,
                    .messageLine2 = "Press S3 to cont" ,
                    .messageTime = "Time 00: 00: 00 " ,
                    .messageADC = " Pot = 0.00 Vdc "
} ;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
int main ( void )
{
    /* Call the System Intialize routine*/
    SYS_Initialize ( ) ;

    /*Initialize Timer*/
    TIMER_SetConfiguration ( TIMER_CONFIGURATION_RTCC ) ;

    TRISGbits.TRISG7 = 1; //in breakout pin RF2 (row 1)
    TRISGbits.TRISG2  = 1; //in breakout pin RA2 (row 2)
    TRISCbits.TRISC0 = 1; //in breakout pin RB8 (row 3)
    TRISCbits.TRISC1 = 1;//in breakout pin RB9 (row 4)                 
    TRISEbits.TRISE12 = 0; //in breakout pin RB12 (col 1)
    TRISCbits.TRISC2 = 0; //in breakout pin RB10 (col 2)
    TRISEbits.TRISE14 = 0; //in breakout pin RB14 (col 3)
    
    
    ANSELGbits.ANSG7 = 0; //Row 1-4        //configuring all pins as digital
    ANSELGbits.ANSG2 = 0;
    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;
    ANSELEbits.ANSE12 = 0; //Col 1-3
    ANSELCbits.ANSC2 = 0;
    ANSELEbits.ANSE14 = 0;
    
    char return_key;
    
    /* Infinite Loop */
    while ( 1 )
    {  
        return_key = 'X'; //initialize return key to default return variable from readKeyboard()
        
        while(return_key == 'X'){ //while nothing is pressed, keep on making calls
            return_key = readKeyboard();
        }
        
        if(return_key != 'X'){ //if return_key is not default key, print to LCD 
            //should transmit to UART at this point  
            LCD_PutChar(return_key);
        }
        
        while(readKeyboard() != 'X'){
            //no operation till next key is pressed.
        }
    }    
}

char readKeyboard()
{
    unsigned int i = 0;
 
    for(i=0;i<3;i++)
     {
       if(i == 0){//column 1 is high
       PORTEbits.RE12 = 1;
       PORTCbits.RC2 = 0;
       PORTEbits.RE14 = 0;
       }
       if(i == 1){//column 2 is high
       PORTEbits.RE12 = 0;
       PORTCbits.RC2= 1;
       PORTEbits.RE14 = 0;
       }
       if(i == 2){//column 3 is high
       PORTEbits.RE12 = 0;
       PORTCbits.RC2 = 0;
       PORTEbits.RE14 = 1;
       }
       
       if((i==0) && PORTGbits.RG7){
        return '1';
       }
       
       if((i==1) && PORTGbits.RG7){
        return '2';
       }
       
       if((i==2) && PORTGbits.RG7){
        return '3';
       }
       
       if((i==0) && PORTGbits.RG2){
        return '4';
       }
       
       if((i==1) && PORTGbits.RG2){
        return '5';
       }
       
       if((i==2) && PORTGbits.RG2){
        return '6';
       }
       
       if((i==0) && PORTCbits.RC0){
        return '7';
       }
       
       if((i==1) && PORTCbits.RC0){
        return '8';
       }
       
       if((i==2) && PORTCbits.RC0){
        return '9';
       }
       
       if((i==0) && PORTCbits.RC1){
        return '*';
       }
       
       if((i==1) && PORTCbits.RC1){
        return '0';
       }
       
       if((i==2) && PORTCbits.RC1){
        return '#';
       }
    }
    return 'X';
}

  