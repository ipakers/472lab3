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

#ifndef _INITIALIZE_H

    #include "inc/lm3s8962.h"

    extern void CPUcpsid();

    extern void CPUcpsie();

    void LED_init();

	void ADC_init();

	void timer0_init();

	void timer1_init();

#endif