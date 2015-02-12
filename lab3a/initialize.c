//*****************************************************************************
/* 
    File Name: initialize.c

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

// initialize the LED
void LED_init() {
    //
    // clock port F
    //
    SYSCTL_RCGC2_R |= 0x00000020;
    for(int i = 0; i < 500; i++);
    // sets the 0th bit of the regester to be an output pin
    GPIO_PORTF_DIR_R &= 0xFFFFFF00;
    GPIO_PORTF_DIR_R |= 0x00000001;
    //Disable other functions by settinng all bits except 7th bit to 0 
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    // enable the 0th bit in the digital enable regester
    GPIO_PORTF_DEN_R &= 0xFFFFFF00;
    GPIO_PORTF_DEN_R |= 0x00000001;
    
    // Turn on the LED.
    //GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
    //GPIO_PORTF_DATA_R |= 0x00000001;
}

void timer0_init(){
    SYSCTL_RCGC1_R |= 0x00010000;

    for(int i = 0; i < 500; i++);
    TIMER0_CTL_R &= 0xFFFFFEFE;                  // disable timerA 
    TIMER0_CFG_R |= TIMER_CFG_32_BIT_TIMER;
    TIMER0_TAMR_R |=  TIMER_TAMR_TAMR_PERIOD;
    TIMER0_TAILR_R &= 0x00;                // set timer to count down from ...
    TIMER0_TAILR_R |= 0x000FFFFF;
    TIMER0_IMR_R |= 0x01;                  // enables timeout interrupts
    // enable debug stall
    TIMER0_CTL_R |= TIMER_CTL_TASTALL;                 
    NVIC_EN0_R |= NVIC_EN0_INT19;
    //Tell core to trigger timer0 interrupt
    // enables timerA
    TIMER0_CTL_R |= 0x01; 
}


void timer1_init(){
    SYSCTL_RCGC1_R |= 0x00020000;
    for(int i = 0; i < 500; i++);
    TIMER1_CTL_R &= 0xFFFFFEFE;                  // disable timerA and timerB
	TIMER1_CFG_R |= TIMER_CFG_32_BIT_TIMER;
	TIMER1_TAMR_R |=  TIMER_TAMR_TAMR_PERIOD;
    TIMER1_TAILR_R &= 0x00;                // set timer to count down from ...
    TIMER1_TAILR_R |= 0x00FFFFFF;
	TIMER1_IMR_R |= 0x01;                  // enables timeout interrupts
    // enable debug stall
    TIMER1_CTL_R |= TIMER_CTL_TASTALL;                 
    NVIC_EN0_R |= NVIC_EN0_INT21;
	//Tell core to trigger timer0 interrupt
    // enables timerA
    TIMER1_CTL_R |= 0x01; 
}


void ADC_init() {
    SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC;
    for (int i = 0; i < 500; i++);
    ADC0_ACTSS_R |= ADC_ACTSS_ASEN0;
    for (int i = 0; i < 500; i++);
}
