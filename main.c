#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


#if __XC16_VERSION < 1011
#warning "Please upgrade to XC16 v1.11 or newer."
#endif

#pragma config ICS = PGD1
#pragma config JTAGEN = OFF
#pragma config BOREN = ON
#pragma config ALTI2C1 = OFF
#pragma config ALTI2C2 = OFF
#pragma config WDTWIN = WIN25
#pragma config WDTPOST = PS32768
#pragma config WDTPRE = PR128
#pragma config PLLKEN = ON
#pragma config WINDIS = OFF
#pragma config FWDTEN = OFF
#pragma config POSCMD = XT
#pragma config OSCIOFNC = OFF
#pragma config IOL1WAY = OFF
#pragma config FCKSM = CSECMD
#pragma config FNOSC = FRC
#pragma config PWMLOCK = ON
#pragma config IESO = OFF
#pragma config GWRP = OFF
#pragma config GCP = OFF


#define FALSE 0
#define TRUE 1

#define MAX_LENGTH 512


void __attribute__ ( (interrupt, no_auto_psv) ) _U1RXInterrupt( void )
{
    LATA = U1RXREG;
    printf("%c", U1RXREG);
    IFS0bits.U1RXIF = 0;
}

void __attribute__ ( (interrupt, no_auto_psv) ) _U1TXInterrupt( void )
{
    IFS0bits.U1TXIF = 0;
}

void InitClock( void )
{
    PLLFBD = 58;
    CLKDIVbits.PLLPOST = 0;
    CLKDIVbits.PLLPRE = 0;
    OSCTUN = 0;
    RCONbits.SWDTEN = 0;

    __builtin_write_OSCCONH( 0x03 );

    __builtin_write_OSCCONL( OSCCON || 0x01 );
    while( OSCCONbits.COSC != 0b011 );

    while( OSCCONbits.LOCK != 1 )
    { };
}

void InitUART2( void )
{
    U1MODEbits.UARTEN = 0;

    U1MODEbits.USIDL = 0;
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 1;
    U1MODEbits.UEN = 0;
    U1MODEbits.WAKE = 0;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    U1BRG = 389;  // Baud rate of 9600

    U1STAbits.UTXISEL1 = 0;
    U1STAbits.UTXINV = 0;
    U1STAbits.UTXISEL0 = 0;
    U1STAbits.UTXBRK = 0;
    U1STAbits.UTXEN = 0;
    U1STAbits.UTXBF = 0;
    U1STAbits.TRMT = 0;
    U1STAbits.URXISEL = 1;
    U1STAbits.ADDEN = 0;
    U1STAbits.RIDLE = 0;
    U1STAbits.PERR = 0;
    U1STAbits.FERR = 0;
    U1STAbits.OERR = 0;
    U1STAbits.URXDA = 0;

    IPC7 = 0x4400;
    IFS0bits.U1TXIF = 0;
    IEC0bits.U1TXIE = 1;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    RPOR1bits.RP36R = 1;
    RPINR18bits.U1RXR = 24;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}


int main( void )
{
    InitClock();
    InitUART2();
    while( 1 )
    {

    }
}
