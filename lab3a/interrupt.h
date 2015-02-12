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

#ifndef _INTERRUPT

    #define SCALE 0x3E8
    #define OFFSET 0x0010EFF0

    void Timer0IntHandler();
    void Timer1IntHandler();

#endif