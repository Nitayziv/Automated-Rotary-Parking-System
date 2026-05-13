
// RC522 RFID Reader
// SDA (SS) : connected to GPC8 (SPI1_SS)
// SCK (SCK) : connected to GPC9 (SPI1_CLK)
// MOSI : connected to GPC11(SPI1_MOSI)
// MISO : connected to GPC10(SPI1_MISO)
// IRQ : no connection
// GND : connected to Gnd
// RST : connected to 3.3V / no connection
// VCC : connected to 3.3V


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSPI.h"
#include "SPI_RC522.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
 

#define MAX_UIDS 4  //  for four cards
#define UID_SIZE 4   // each uid is 4 bytes


 
unsigned char UID[4]={ 0 },Temp[4]                               ;
unsigned char RF_Buffer[18]                                        ;
unsigned char Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}   ; // Mifare One ????
int out_flag, ok_flag,uid_flag = 0;// uid_flag = 0 enable the option for new users
volatile int read_flag=0; 
char Text[16]; 
char lastScannedUID[9] = ""; // 8 characters + null terminator

uint8_t  Timer_flag =0;
uint32_t Timer_count =0;

unsigned char uid_list[MAX_UIDS][UID_SIZE]; // 2D Array
int uid_count = 0; 

char uidString[9];  // 8 characters + null terminator

void Init_SPI()
{
	DrvSPI_Open(eDRVSPI_PORT1, eDRVSPI_MASTER, eDRVSPI_TYPE1, 8);
	DrvSPI_SetEndian(eDRVSPI_PORT1, eDRVSPI_MSB_FIRST);
	DrvSPI_DisableAutoSS(eDRVSPI_PORT1);
	DrvSPI_SetClockFreq(eDRVSPI_PORT1, 50000, 0); // set SPI clock = 50KHz
}


//reading func that uses the driver's func
//void Reader(void)
//{
//    int temp;

//    if(PcdRequest(0x52, Temp) == MI_OK) // check if there any card on the sensor
//    {
//        if(PcdAnticoll(UID) == MI_OK) 
//        {
//					  DrvSYS_Delay(1000);
//            read_flag = 1; // signaling that the readin is finished
//					 
//        }
//        else 
//        {
//            print_lcd(1, "Try again!      ");
//					  DrvSYS_Delay(1000);
//            read_flag = 0; // when it was unseccsesful
//					 
//        }
//			}
//    else 
//    { 

//        read_flag = 0; // when it was unseccsesful

//			 
//    } 
//			
//}












// check if the uid already exisits
int is_uid_exists(unsigned char uid[UID_SIZE]) {
	int i;
    for (i = 0; i < uid_count; i++) 
	{
        if (memcmp(uid_list[i],uid, UID_SIZE) == 0) {
            return 1; 
        }
    }
	uid_flag = 1; //let the  new user to be registerd
    return 0; 
}



//adding new client
void add_uid(unsigned char uid[UID_SIZE]) {
	
    if (uid_flag == 0 && is_uid_exists(uid)) 
			{
        print_lcd(1,"User exist! :           ");
        DrvSYS_Delay(1000000);				
        return;
    }
    if (uid_flag == 1 && uid_count < MAX_UIDS) {
        memcpy(uid_list[uid_count], uid, UID_SIZE);
        uid_count++;
			  uid_flag = 0;
        print_lcd(1,"Welcome! :     ");
			  DrvSYS_Delay(1000000);
			  return;
    }
	
		else if(uid_count == MAX_UIDS){
        print_lcd(1,"Sorry we are full! :     ");
			  DrvSYS_Delay(1000000);
		    return;
    }
}



////this function will get new UID for new user.
//void GetUID(void)
//{
//    Reader();  // Reading
//    
//    if (read_flag)  
//    {
//        add_uid(UID);
//        read_flag=0;
//    }
//	    
//}







//int main(void)
//{
//	
//	char TEXT[16]="               ";	
//	
//	UNLOCKREG();
//	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1); // Enable the 12MHz oscillator oscillation
//	DrvSYS_SelectHCLKSource(0); // HCLK clock source. 0: external 12MHz; 4:internal 22MHz RC oscillator
//	LOCKREG();
//	
//	
//	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0); /* HCLK clock frequency = HCLK clock source / (HCLK_N + 1) */
//	DrvGPIO_InitFunction(E_FUNC_SPI1);
//	
//	Init_SPI();
//	Initial_panel(); 
//	clr_all_panel();
//	
//	PcdReset();
//	PcdAntennaOn();
//	
////set interrupts
//// DrvGPIO_Open(E_GPB, 14, E_IO_INPUT);         // initial SWINT button for changing series
//// DrvGPIO_EnableEINT0(E_IO_RISING,E_MODE_EDGE,Reader); // set up Interrupt callback	

//	while (1)
//	{
//		
//		 GetUID();  // call function for reading rfid tag



//		sprintf(TEXT, "%x %x %x %x" , UID[0], UID[1], UID[2], UID[3]);
//		print_lcd(3,TEXT);
//		DrvSYS_Delay(100000);
//	}



// function that converts UID into string//------------------------------------------------------------------------------------
void convertUIDToString(unsigned char *uid, char *uidString) {
    sprintf(uidString, "%02X%02X%02X%02X", uid[0], uid[1], uid[2], uid[3]);
}
//---------------------------------------------------------------------------------------------------------------------------------//

void Reader(void)
{
    

    if (PcdRequest(0x52, Temp) == MI_OK) // Check if card is detected
    {
        if (PcdAnticoll(UID) == MI_OK) // Read the UID
        { 
            convertUIDToString(UID, uidString); // Convert UID to string

            print_lcd(1, "Card Detected:");
            print_lcd(2, uidString);  // Display UID string on LCD

            read_flag = 1;  // Mark read as successful
        }
        else 
        {
            print_lcd(2, "UID Read Error"); // Error reading UID
        }
    }
    else 
    {
        print_lcd(1, "No Card Found"); // No card detected
    }
}





