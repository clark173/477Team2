#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "app.h"

#define ONE_VOLT 310
#define ONE_TENTH_VOLT 31
#define ONE_HUNDREDTH_VOLT 3

void SYS_Initialize ( void ) ;

APP_DATA appData = {
                    .messageLine1 = "Explorer 16 Demo" ,
                    .messageLine2 = "Press S3 to cont" ,
                    .messageTime = "Time 00: 00: 00 " ,
                    .messageADC = " Pot = 0.00 Vdc "
} ;

void Write_SPI ( short command )
{
   // int16_t temp;

    PORTGbits.RG15 = 0;                         // lower the slave select line
    //temp = SPI2BUF;                           // dummy read of the SPI2BUF register to clear the SPIRBF flag
    while(SPI2STATbits.SRMPT == 1 && SPI2STATbits.SPITBF == 0);            // wait for the SPI to be valid
    SPI2BUF = command;                          // write the data out to the SPI peripheral
    PORTGbits.RG15 = 1;                         // raise the slave select line
}

/******************************************************************************
 * Function:        void Delay(void))
 *
 * PreCondition:    None   
 *
 * Input:           None
 *                  
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function provides sofware delay
 *****************************************************************************/
void Delay ( void )
{
    int16_t temp;
    for( temp = 0; temp < 255; temp++ );
}

/******************************************************************************
 * Function:        void Init_SPI(void)
 *
 * PreCondition:    None   
 *
 * Input:           None
 *                  
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function configures SPI module
 *****************************************************************************/
void Init_SPI ( void )
{
   // IFS0bits.SPI1IF = 0;
   // IEC0bits.SPI1IE = 0;
    
    // setup the SPI peripheral per DS70005185A section 3.3.1.1
    SPI2CON1bits.MSTEN = 1;
    SPI2STATbits.SPIROV = 0;
    SPI2CON1bits.DISSCK = 0;
    SPI2CON1bits.DISSDO = 0;
    SPI2CON1bits.MODE16 = 1;
    SPI2CON1bits.SMP = 0;
    SPI2CON1bits.CKE = 0;
    SPI2CON1bits.CKP = 0;
    SPI2STATbits.SPIEN = 1;
    
    //IFS0bits.SPI2IF = 0;
  //  IEC0bits.SPI2IE = 1;
}

int main (void)
{
    SYS_Initialize ( );
    
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); //unlock registers
    
    RPINR22bits.SCK2R = 0x39;//RP57 = RC9 (RD7 on breakout) (D0 on LOGIC);
    RPINR22bits.SDI2R = 0x61;//RP97 = RF1 (RF1 on breakout) (D1 on LOGIC);
    
    RPOR7bits.RP57R = 0b001001; //SCKOUT, RP57 = RC9 (RD7 on breakout) (D0 on LOGIC);
    RPOR8bits.RP70R = 0b001000; //SDO, RP70 = RD6 (RD6 on breakout) (D2 on LOGIC);
    
     __builtin_write_OSCCONL(OSCCON | (1<<6)); //lock registers
     
    TRISFbits.TRISF1 = 1;
    TRISCbits.TRISC9 = 0;
    TRISDbits.TRISD6 = 0;
   
    TRISGbits.TRISG15 = 0;
    ANSELD = 0x00;
    ANSELF = 0x00;
    ANSELG = 0x00;
    
    Init_SPI();
    int16_t i;
 
    while( 1 )
    {
        for( i = 0; i < 255; i++ )
        {
            Write_SPI( i );
            //Delay();
        }
    } 
}