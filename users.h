#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvUART.h"
#include "Driver\DrvGPIO.h"
#include "Driver\DrvSPI.h"
#include "SPI_RC522.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
#define MAX_SPOTS 4
#define MAX_INPUT_LENGTH 16
#include "NUC1xx-LB_002\LCD_Driver.h"

// Structure to store parking spots
typedef struct {
    char uid[9];   // 8-character UID string + null terminator
    short parking_spot;   // Parking Spot Number
		bool is_paid; 
} ParkingSpot;

typedef struct {  
    char owner_name[20];                                         
    uint8_t	last4IdDigits;
	  ParkingSpot data;
    char entry_time[20]; // FORMAT (YY-MM-DD HH:MM")
    char exit_time[20];      
} ParkingUser;




// Predefined Parking Spot UIDs (Stored as Strings)
ParkingSpot parkingSpots[] = {
    {"B4F03351", 1, false},  // Spot 1
    {"D5EACAD9", 2, false},  // Spot 2
    {"32E7D5B9", 3, false},  // Spot 3
    {"4DC65A59", 4, false}   // Spot 4
};

ParkingUser users[MAX_SPOTS];
int userCount = 0;

//Declarations 

void initializeParkingSpots(ParkingUser users[]);
int is_user_exists(short parking_num);
void handleUserArrival(void);
bool checkInputLength(const char* input, size_t maxLen);
int findUserIndexByLast4Digits(uint8_t code);
int findUserIndexByName(const char *name);





