//*****************************************************************************
/* 
    File Name: main.c

    Description: 

    Authors: Isaac Perry, Thang Phu and Anji Xu

    Date: 01/29/15
*/
//*****************************************************************************
//
// Table of Contents:
//
// LED_init() - initializes the on board LED. Used primarily for debugging.
//
//
//*****************************************************************************
    
// import macros for the individual regesters;
#include "inc/lm3s8962.h"
// for 32 bit integers;
#include "stdint.h"
// drivers;
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
// peripheral functions;


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine) {
}
#endif

#define TRUE 1
#define FALSE 0


//*****************************************************************************
//
// Flashes A B C D together at a one-second rate.
//
//*****************************************************************************

void ledInitialize();
void timer0Initialize();


int main(void) {

	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    // initialize the peripherals;
    ledInitialize();
    timer0Initialize();
    while(TRUE) {
    }
}

void ledInitialize() {
    // clock port F;
    SYSCTL_RCGC2_R |= 0x00000020;
    // sets the 0th bit of the regester to be an output pin;
    GPIO_PORTF_DIR_R &= 0xFFFFFF00;
    GPIO_PORTF_DIR_R |= 0x00000001;
    //Disable other functions by settinng all bits except 7th bit to 0;
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    // enable the 0th bit in the digital enable regester;
    GPIO_PORTF_DEN_R &= 0xFFFFFF00;
    GPIO_PORTF_DEN_R |= 0x00000001;
    /* Turn on the LED;
    GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
    GPIO_PORTF_DATA_R |= 0x00000001;
    */
}

void timer0Initialize() {
        CPUcpsid();
	// clock the timer
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_TIMER0;
	// disable the timer A
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;
	// set timer to 32-bit configuration
	TIMER0_CFG_R &= TIMER_CFG_32_BIT_TIMER;
	// set timer to periodic mode
	TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;
	// set timer interval to a large possible value
    TIMER0_TAILR_R &= 0x0;
	TIMER0_TAILR_R |= 0x00FFFFFF;
	// enable time out interrupts
	TIMER0_IMR_R |= TIMER_IMR_TATOIM;
    // enable debug stall
    TIMER0_CTL_R |= TIMER_CTL_TASTALL;
	// start the timer
	TIMER0_CTL_R |= TIMER_CTL_TAEN;
	// enable controller interrupts coming from the timer
	NVIC_EN0_R |= NVIC_EN0_INT19;
	CPUcpsie();
    TIMER0_ICR_R |= 0x00000001;
}








/*

#include "inc/lm3s8962.h"
#include "stdio.h"
#include <stdint.h>
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"


void LED_init();
extern void timer0_init();
extern void Timer0IntHandler();

int main(void) {
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);
    LED_init();
    timer0_init();
    while(1){
    }
}

// initialize the LED
void LED_init() {
    //
    // clock port F
    //
    SYSCTL_RCGC2_R |= 0x00000020;
    
    // sets the 0th bit of the regester to be an output pin
    GPIO_PORTF_DIR_R &= 0xFFFFFF00;
    GPIO_PORTF_DIR_R |= 0x00000001;
    
    //Disable other functions by settinng all bits except 7th bit to 0 
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    
    // enable the 0th bit in the digital enable regester
    GPIO_PORTF_DEN_R &= 0xFFFFFF00;
    GPIO_PORTF_DEN_R |= 0x00000001;
    
    // Turn on the LED.
    GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
    GPIO_PORTF_DATA_R |= 0x00000001;
}

*/