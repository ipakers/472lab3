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
#include <stdint.h>
#include "stdio.h"
#include "interrupt.h"

#define SCALE 0x3E8
#define OFFSET 0x0010EFF0

int requestSample = 1;

void Timer0IntHandler() {
    TIMER0_ICR_R |= 0x01;
    if (requestSample) {
        int i = 0;
    	ADC0_PSSI_R = 0x1;
        for (i = 0; i < 500; i++);
        requestSample = 0;
    } else {
    	uint32_t value = ADC0_SSFIFO0_R & ADC_SSFIFO0_DATA_M;
        TIMER1_TAILR_R = OFFSET - (value * SCALE);
        requestSample = 1;
    }

}

void Timer1IntHandler() {
    TIMER1_ICR_R |= 0x01;
    if ((GPIO_PORTF_DATA_R &= 0x00000001) > 0) {
    	GPIO_PORTF_DATA_R &= 0xFFFFFFFE;
    } else {
    	GPIO_PORTF_DATA_R |= 0x00000001;
    }
}