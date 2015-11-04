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

#define BAUD 9600
#define FREQ 60000000  // 60MHz clock frequency
#define MAX_LENGTH 512


// Global variable declarations
int string_index = 0;  // Indicates the current index of the input string from the Rx buffer
char input_string[MAX_LENGTH];

// Flags
char rx_complete = 0;
char uart_end_flag = 0;


// The following interrupt is triggered whenever a byte is received on the Rx
// buffer. It checks for a stop symbol of "*/" sent by the Pi and marks the
// Rx transmission as complete once one is found.
void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    //LATA = U1RXREG;
	if (uart_end_flag == 1)
	{
		if (U1RXREG == '/')
		{
			rx_complete = 1;
			uart_end_flag = 0;
			U1MODEbits.UARTEN = 0;  // Temporarily disable UART for computation
		}
		else
		{
			uart_end_flag = 0;
			rx_complete = 0;
		}
	}
	
	if (U1RXREG == '*')
	{
		uart_end_flag = 1;
	}
	else
	{
		uart_end_flag = 0;
	}
	input_string[string_index++] = U1RXREG;
    IFS0bits.U1RXIF = 0;
}

void __attribute__ ((interrupt, no_auto_psv)) _U1TXInterrupt(void)
{
    IFS0bits.U1TXIF = 0;
}

void InitClock(void)
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

void InitUART2(void)
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

	U1BRG = (FREQ / (16*BAUD)) - 1;
    //U1BRG = 389;  Baud rate of 9600

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

// Seperates all of the individual packets received from Rx and returns to main
char* unpack_data()
{
	char local_data[MAX_LENGTH] = input_string;
	int i;
	
	string_index = 0;  // Clear the global variables
	for (i = 0; i < MAX_LENGTH; i++)
	{
		input_string[i] = '';
	}
	
	return local_data.split('<');
}

// Read the data on the SD card. If it contains the barcode, update it
// Otherwise, add it to the file
void read_sd_card(char* packets)
{
	
}

// Send information back to the Raspberry Pi
void send_uart_data(char* package)
{
	U1MODEbits.UARTEN = 1;  // Re-enable UART if not already done

	printf("%s", package);
}


int main(void)
{
	// packets[0] = UPC-A Barcode
	// packets[1] = Item Name
	// packets[2] = Number of Servings
	// packets[3] = Scan Date (MM/DD/YYYY);
	char* packets;

    InitClock();
    InitUART2();
    while(TRUE)
    {
		if (rx_complete)  // Marked true when a stop word is found
		{
			rx_complete = 0;
			packets = unpack_data();

			if (len(packets) > 1)  // Invalid barcode if == 1
			{
				read_sd_card(packets);
			}
			else
			{
				U1MODEbits.UARTEN = 1;  // Enable UART again for new package
			}
		}
    }
}