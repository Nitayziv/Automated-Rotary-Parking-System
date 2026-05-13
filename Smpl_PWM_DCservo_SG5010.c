//
// Smpl_PWM_DCservo_SG5010
//
// using PWM to generate 50Hz (20ms) pulse to DC Servo signal pin
// 0.5 ~ 2.5ms high time (PWM clock at 10us per count)

// SG5010 DC servo
// pin1 : signal to PWM0/GPA12 (NUC140-pin65/NUC120-pin28)
// pin2 : Vcc
// pin3 : Gnd

#include <stdio.h>																											 
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#include "NUC1xx-LB_002\LCD_Driver.h"
#include "Driver_PWM_Servo.h"

#define HITIME_MIN 50  // 0.5ms
#define HITIME_MAX 250 // 2.5ms

uint8_t hitime;

           

  
void OpenGate(){
	InitPWM(0); // initialize PWM0
for (hitime=HITIME_MIN; hitime<=HITIME_MAX; hitime++) {
		PWM_Servo(0, hitime);	
    }
}

