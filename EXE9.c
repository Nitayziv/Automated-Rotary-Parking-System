
// Smpl_UART0_HC05 : bluetooth module receive 8 bytes and display on LCD
//
// HC05 Bluetooth module
// pin1 : KEY   N.C
// pin2 : VCC   to Vcc +5V
// pin3 : GND   to GND
// pin4 : TXD   to NUC140 UART0-RX (GPB0)
// pin5 : RXD   to NUC140 UART0-TX (GPB1)
// pin6 : STATE N.C.

#include <stdio.h>
#include <string.h>
#include "NUC1xx.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSYS.h"
#include "NUC1xx-LB_002\LCD_Driver.h"

#define MAX_INPUT_LENGTH 16

short key;

// Initial GPIO pins (GPC 12,13,14,15) to Output mode  
void Init_LED()
{
	// initialize GPIO pins
	DrvGPIO_Open(E_GPC, 12, E_IO_OUTPUT); // GPC12 pin set to output mode
	DrvGPIO_Open(E_GPC, 13, E_IO_OUTPUT); // GPC13 pin set to output mode
	DrvGPIO_Open(E_GPC, 14, E_IO_OUTPUT); // GPC14 pin set to output mode
	DrvGPIO_Open(E_GPC, 15, E_IO_OUTPUT); // GPC15 pin set to output mode
	// set GPIO pins to output Low
	DrvGPIO_SetBit(E_GPC, 12); // GPC12 pin output Hi to turn off LED
	DrvGPIO_SetBit(E_GPC, 13); // GPC13 pin output Hi to turn off LED
	DrvGPIO_SetBit(E_GPC, 14); // GPC14 pin output Hi to turn off LED
	DrvGPIO_SetBit(E_GPC, 15); // GPC15 pin output Hi to turn off LED
}

void RGB_LED()
{
	// initialize GPIO pins
	DrvGPIO_Open(E_GPA, 12, E_IO_OUTPUT); // GPA12 pin set to output mode
	DrvGPIO_Open(E_GPA, 13, E_IO_OUTPUT); // GPA13 pin set to output mode
	DrvGPIO_Open(E_GPA, 14, E_IO_OUTPUT); // GPA14 pin set to output mode
	// set GPIO pins output Hi to disable LEDs
	DrvGPIO_SetBit(E_GPA, 12); // GPA12 pin output Hi to turn off Blue  LED
	DrvGPIO_SetBit(E_GPA, 13); // GPA13 pin output Hi to turn off Green LED
	DrvGPIO_SetBit(E_GPA, 14); // GPA14 pin output Hi to turn off Red   LED
}    





char TEXT[MAX_INPUT_LENGTH];
volatile uint8_t comRbuf[9];
volatile uint8_t comRbytes = 0;
uint8_t  dataout1[4] = "\r\nOK";
uint8_t dataout2[30] = "\r\nINSERT YOUR TICKET NUMBER!";
uint8_t flag=0;

//use INT for sending data for uart0 channel
void UART_INT_HANDLE(void)
{
	while(UART1->ISR.RDA_IF==1) 
	{
		comRbuf[comRbytes]=UART1->DATA;
		comRbytes++;		
		if (comRbytes==1) {	
			sprintf(TEXT,"%s",comRbuf);
			//print_lcd(1,TEXT);			
		  comRbytes=0;
		}
	}
}




int32_t main()
{
	uint32_t distance;
	uint8_t  write_byte[1];
	uint8_t  read_byte[2];
	STR_UART_T sParam;

	UNLOCKREG();
  DrvSYS_Open(48000000);
	LOCKREG();
  Init_LED();  // Initialize LEDs (four on-board LEDs below LCD panel)
  RGB_LED();	
		// Set UART pins

	/* UART Setting */
    sParam.u32BaudRate 		  = 9600;
    sParam.u8cDataBits 		  = DRVUART_DATABITS_8;
    sParam.u8cStopBits 		  = DRVUART_STOPBITS_1;
    sParam.u8cParity 		    = DRVUART_PARITY_NONE;
    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;

	   	/* Set UART Configuration */
	//for the BT device
	DrvGPIO_InitFunction(E_FUNC_UART1);
 	if(DrvUART_Open(UART_PORT1,&sParam) != E_SUCCESS);
	DrvUART_EnableInt(UART_PORT1, DRVUART_RDAINT,UART_INT_HANDLE);
	//for the SR04 sensor
	DrvGPIO_InitFunction(E_FUNC_UART0);
	if(DrvUART_Open(UART_PORT0,&sParam) != E_SUCCESS);
	
	Initial_panel();                 // initialize LCD panel
	clr_all_panel();                 // clear LCD panel							 	
	
	
	while(1)
	{
//		write_byte[0]=0x55;						// trigger SRF04
//		DrvUART_Write(UART_PORT0,write_byte,1);	// write to SRF04
//		DrvUART_Read(UART_PORT0,read_byte,2); 	// read two bytes from SRF04
//		distance = read_byte[0]*256 + read_byte[1];// distance = byte[0] *256 + byte[1];
//		if(distance <= 50 ){
//		 handleUserArrival();
//		exitfunction();
		key = 2;
		bringSpotDown(key);
		key = 4;
		bringSpotDown(key);
		break;
		}
		
///////////////////////////////////////////////////////////////////////////////////						 	
		  
	
	//DrvUART_Close(UART_PORT0);
}
	

}
