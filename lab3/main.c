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

#include "inc/lm3s8962.h"
#include "stdio.h"
#include <stdint.h>
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/cpu.h"


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