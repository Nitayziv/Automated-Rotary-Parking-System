#include "users.h"
#include "Driver_PWM_Servo.h"
extern char TEXT[16];

	
extern char uidString[9];
char spotUID[9];


void Reader();
void CW_MOTOR(uint16_t deg);
void DrvSYS_Delay(uint32_t us);
void bringSpotDown(short parking_spot);

#define HITIME_MIN 50  // 0.5ms
#define HITIME_MAX 250 // 2.5ms

int i;
uint8_t hitime;

void OpenGate(){
	DrvGPIO_SetBit(E_GPA, 12); // GPA12 pin output Hi to turn off Blue  LED
	DrvGPIO_SetBit(E_GPA, 13); // GPA13 pin output Hi to turn off Green LED
	DrvGPIO_SetBit(E_GPA, 14); // GPA14 pin output Hi to turn off Red   LED
	DrvGPIO_ClrBit(E_GPA, 13);
	for (hitime=HITIME_MIN; hitime<=HITIME_MAX; hitime++) {
		PWM_Servo(0, hitime);	
		DrvSYS_Delay(100000);
	}
	DrvGPIO_SetBit(E_GPA, 12); // GPA12 pin output Hi to turn off Blue  LED
	DrvGPIO_SetBit(E_GPA, 13); // GPA13 pin output Hi to turn off Green LED
	DrvGPIO_SetBit(E_GPA, 14); // GPA14 pin output Hi to turn off Red   LED
	DrvGPIO_ClrBit(E_GPA, 14);
}






// Function to Get UID of a Parking Spot
char* getUIDByParkingSpot(short parking_spot) {
    int totalSpots = 4;
    int i;

    for (i = 0; i < totalSpots; i++) {
        if (parkingSpots[i].parking_spot == parking_spot) {
            strcpy(spotUID, parkingSpots[i].uid);  // Copy UID into global buffer
            return spotUID;  // Return pointer to modifiable string
        }
    }

    return NULL;  // Return NULL if the parking spot is not found
}

void textToLCD(const char *format, int line) {
    sprintf(TEXT, "");  // Clear previous text
    sprintf(TEXT, format);  // Format and store the text
    print_lcd(line, TEXT);  // Display on LCD
}

void bringSpotDown(short parking_spot){
	char* uid;
	uid = getUIDByParkingSpot(parking_spot);
	print_lcd(0,uid);
	
	while(1){
		Reader();
		if (strncmp(uidString, uid, 2) == 0){
			print_lcd(3, "good");
			break;  // Stop rotating and exit function
		}
		else{
			CW_MOTOR(45); // Clockwise
			DrvSYS_Delay(2000000);
		}

	}
	
}

short freeParkingSpot() {
    char TEXT[20];

    for (i = 0; i < 4; i++) {
        if (!parkingSpots[i].is_paid) {  // Check if spot is available
						parkingSpots[i].is_paid = true;
            return parkingSpots[i].parking_spot;  // Return first free spot number
        }
    }

    // If no free spots are found, display "PARKING FULL" on the LCD
    clr_all_panel();
    sprintf(TEXT, "");  // Clear previous text
    sprintf(TEXT, "PARKING FULL");  
    print_lcd(1, TEXT);

    return -1;  // Indicate that no free spot is available
}

// Function to mark a parking spot as free
void clientOut(short parking_spot) {
    for (i = 0; i < 4; i++) {
        if (parkingSpots[i].parking_spot == parking_spot) {  // Find the matching spot
            parkingSpots[i].is_paid = false;  // Mark as free
            return;
        }
    }
}




void entrance(){
	int SPOTin;
  SPOTin = freeParkingSpot();
	
	clr_all_panel();
	
	sprintf(TEXT,"");
	sprintf(TEXT, "your spot is %d", SPOTin);
	print_lcd(1,TEXT);
	
	OpenGate();
	clr_all_panel();
	DrvSYS_Delay(50000);
	bringSpotDown(SPOTin);
	
}	
int SPOTout = 1;
void clientExit(){
		clr_all_panel();
		textToLCD("enter your spot",0);
//		while (1){
//			SPOTout = Scankey();
//			if(SPOTout != 0){
////				bringSpotDown(SPOTout);
//					sprintf(TEXT,"");
//					sprintf(TEXT, "your spot is %d", SPOTout);
//					print_lcd(1,TEXT);
//				break;
//			}
//			clr_all_panel();
//			textToLCD("GOODBYE",0);
//		}
		clientOut(SPOTout);
		bringSpotDown(SPOTout);
		SPOTout++;
		if (SPOTout>=5)
			SPOTout = 1;
		DrvSYS_Delay(500000);
		OpenGate();
}
		



