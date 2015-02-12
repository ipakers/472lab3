/*

//*****************************************************************************
/* 
    File Name: main.c

    Description: The high level controller for lab 2. Prints a menu system to 
    a peripheral LCD display. The user presses arrow keys and a select key to 
    navigate the menu system.

    Authors: Isaac Perry, Thang Phu and Anji Xu

    Date: 01/29/15

//*****************************************************************************
//
// Table of Contents:
//
// LED_init() - initializes the on board LED. Used primarily for debugging.
//
// LED_toggle() - blinks the led once at about a one second rate.
//
// display_menu(int* myPtr) - prints the main menu on the LCD display.
//      myPtr is a reference to the current location of the cursor on the 
//      display.
//
// delay(unsigned long aValue) - pauses the execution of the program. 
//     aValue is the number of clock cycles that the program pauses. The 
//     software needs to pause in order for the characters to become visible.
//
//*****************************************************************************
    
// import macros for the individual regesters;
#include "inc/lm3s8962.h"
// for IO functions;
#include "stdio.h"
// for 32 bit integers;
#include <stdint.h>
// for string methods;
#include "string.h"
// drivers;
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
// peripheral functions;
#include "keypad.h"
#include "display.h"
#include "advanced_display.h"


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

void LED_init();
void LED_toggle();
void delay(int aDelay);
void display_menu(int* myPtr);

// global variable which stores the previous key press;
// updated when a new key is pressed;
// used to make sure a key which is held down is not interpreted as multiple
//    button presses;
uint32_t prevKey = 0;



int main(void) {
    // cursor and menu state;
   int cursorPos = 0;
   int* myPtr = &cursorPos;
   int whichMenu = 0;
  
    //
    // Set the clocking to run directly from the crystal;
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    // initialize the peripherals;
    LED_init();
    key_init();
    display_init();
    display_menu(myPtr);
    
    // turn off LED from initialization process;
    GPIO_PORTF_DATA_R &= 0xFFFFFFFC;


    while(TRUE) {
        int i = 0;
        // check for a key press;
        int key = keyMaster();

        // display the code for the pressed character on the LED;
        for (i = 0; i < key; i++) {
            LED_toggle();
            delay(1000);
        }
        
        if ((key == 1) && (*myPtr >= 40)) { 
            // UP button;
            // move the cursor up to the first line;
            // cursor must be on the second line;
            // otherwise does nothing;
            volatile unsigned long i = 0;
            for (i = 0; i < 40; i++) {
                cd_shift(0x00);
            }
            cursorPos = *myPtr;
            cursorPos = cursorPos - 40;
            *myPtr = cursorPos;//update cursorPos;
        } else if ((key == 2) && (*myPtr < 40)) { 
            // DOWN button;
            // move the cursor down to the second line;
            // cursor must be on the first line;
            // otherwise does nothing;
            volatile unsigned long i = 0;
            for (i = 0; i < 40; i++) {
                cd_shift(0x01);
            }
            cursorPos = *myPtr;
            cursorPos = cursorPos + 40;
            *myPtr = cursorPos;//update cursorPos;
        } else if ((key == 3)  && (whichMenu == 1)) { 
            // LEFT button;
            // go back to main menu if it's in submenu;
            clear_display();
            display_menu(myPtr);
            whichMenu = 0; //update the whichMenu status;
        } else if ((key == 4) && (whichMenu == 0)) { 
            // Right button;
            // Go to the submenu from the main menu;
            // must be in the main menu, otherwise does nothing;
            clear_display();  
            if (*myPtr < 40){
                display_string("BEEF: $10/lb");
            } else {
                display_string("PORK: $5/lb");
            }
            whichMenu = 1; //update the whichMenu status;
        } else if ((key == 5) && (whichMenu == 1)) { 
            // Select button;
            // Go to the comfirmation if in a submenu;
            // otherwise does nothing;
            clear_display();  
            display_string("Transaction");
            volatile unsigned long i = 0;
            for (i = 0; i < 40 - 11; i++) {
                cd_shift(0x01);
            }
            display_string("Complete");
        }
    }
}
//  software delay;
void delay(int aDelay) {
    volatile unsigned long i = 0;

    volatile unsigned int j = 0;
    
    for (i = aDelay; i > 0; i--) {
        for (j = 0; j < 100; j++);
    }
}

// initialize the LED;
//
// POST: 
//    regesters are configured to handle the LED;
void LED_init() {
    //
    // clock port F;
    //
    SYSCTL_RCGC2_R |= 0x00000020;
    
    // sets the 0th bit of the regester to be an output pin;
    GPIO_PORTF_DIR_R &= 0xFFFFFF00;
    GPIO_PORTF_DIR_R |= 0x00000001;
    
    //Disable other functions by settinng all bits except 7th bit to 0;
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    
    // enable the 0th bit in the digital enable regester;
    GPIO_PORTF_DEN_R &= 0xFFFFFF00;
    GPIO_PORTF_DEN_R |= 0x00000001;
    
    // Turn on the LED;
    GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
    GPIO_PORTF_DATA_R |= 0x00000001;
}

// turns on the LED and then turns it off at about a one second rate;
void LED_toggle() {
  // Turn on the LED;
    GPIO_PORTF_DATA_R |= 0x00000001;
    
    delay(1000);
    
    // Turn on the LED;
    GPIO_PORTF_DATA_R &= 0xFFFFFFFE;
    
    delay(1000);
}


// PRE:
//      myPtr - a pointer to the current position of the display cursor;
// writes to the display the main menu;
//
// POST:
//      the main menu is written to the display;
void display_menu(int* myPtr) {
    char* line1="BEEF";
    char* line2="PORK";
    
    *myPtr=0;
    // Write the 1st line;
    display_string(line1);
    *myPtr=strlen(line1)-1;
    
    // move the cursor to the beginning of the next line;
    volatile unsigned long i = 0;
    for (i = 0; i < 40 - strlen(line1); i++) {
        cd_shift(0x01);
    }
    
    *myPtr = 40;
    
    // Write the 2nd line;
    display_string(line2);
    *myPtr=40+strlen(line2);
}
*/

//*****************************************************************************
/* 
    File Name: main.c

    Description: The high level controller for lab 2. Prints a menu system to 
    a peripheral LCD display. The user presses arrow keys and a select key to 
    navigate the menu system.

    Authors: Isaac Perry, Thang Phu and Anji Xu

    Date: 01/29/15
*/
//*****************************************************************************
//
// Table of Contents:
//
// LED_init() - initializes the on board LED. Used primarily for debugging.
//
// LED_toggle() - blinks the led once at about a one second rate.
//
// display_menu(int* myPtr) - prints the main menu on the LCD display.
//      myPtr is a reference to the current location of the cursor on the 
//      display.
//
// delay(unsigned long aValue) - pauses the execution of the program. 
//     aValue is the number of clock cycles that the program pauses. The 
//     software needs to pause in order for the characters to become visible.
//
//*****************************************************************************
    
// import macros for the individual regesters;
#include "inc/lm3s8962.h"
// for IO functions;
#include "stdio.h"
// for 32 bit integers;
#include <stdint.h>
// for string methods;
#include "string.h"
// drivers;
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
// peripheral functions;
#include "keypad.h"
#include "display.h"
#include "advanced_display.h"
#include "driverlib/interrupt.h"

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

void LED_init();
void LED_toggle();
void delay(int aDelay);
void display_menu(int* myPtr);
/////////////////////////////////////////////////////////
void timer0Initialize();
void Timer0IntHandler();
void processKey(uint32_t key);


// global variable which stores the previous key press;
// updated when a new key is pressed;
// used to make sure a key which is held down is not interpreted as multiple
//    button presses;
uint32_t prevKey = 0;

///////////////////////////////////////////////////////////
uint32_t curKey = 0;
int flag = 1;
int cursorPos = 0;
int* myPtr = &cursorPos;
int whichMenu = 0;

int main(void) {
    // cursor and menu state;

  
    //
    // Set the clocking to run directly from the crystal;
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);
    delay(100);
    // initialize the peripherals;
    LED_init();
    key_init();
    display_init();
    display_menu(myPtr);
    /////////////////////////////////////////////////////////
    timer0Initialize();

    while(TRUE);
}

////////////////////////////////////////////////////
void processKey(uint32_t key) {

    if ((key == 1) && (*myPtr >= 40)) { //Up: go to the same position of the previous line
                                        //but only when it's on the second line
        volatile unsigned long i = 0;
        for (i = 0; i < 40; i++) {
            cd_shift(0x00);
        }
        cursorPos = *myPtr;
        cursorPos = cursorPos - 40;
        *myPtr = cursorPos;//update cursorPos
    }
        
    if ((key == 2) && (*myPtr < 40)) { //Down: go to the same position of the nextline
                                       //but only when it's on the first line
        volatile unsigned long i = 0;
            for (i = 0; i < 40; i++) {
                cd_shift(0x01);
            }
        cursorPos = *myPtr;
        cursorPos = cursorPos + 40;
        *myPtr = cursorPos;//update cursorPos
    } 
        
    if ((key == 3)  && (whichMenu == 1)) { //Left: go back to main menu if it's in submenu
        clear_display();
        display_menu(myPtr);
        whichMenu = 0; //update the whichMenu status
    }
    
    if ((key == 4) && (whichMenu == 0)) { //Right: 
        clear_display();  //Go to the submenu
        if (*myPtr < 40){
            display_string("BEEF: $10/lb");
        } else {
            display_string("PORK: $5/lb");
        }
        whichMenu = 1; //update the whichMenu status
    }
        
    if ((key == 5) && (whichMenu == 1)) { //Select: 
        clear_display();  //Go to the comfirmation
        display_string("Transaction");
    volatile unsigned long i = 0;
    for (i = 0; i < 40 - 11; i++) {
        cd_shift(0x01);
    }
        display_string("Complete");
    }

}

// initialize the LED;
//
// POST: 
//    regesters are configured to handle the LED;
void LED_init() {
    //
    // clock port F;
    //
    SYSCTL_RCGC2_R |= 0x00000020;
    for (int i = 0; i < 500; i++);
    // sets the 0th bit of the regester to be an output pin;
    GPIO_PORTF_DIR_R &= 0xFFFFFF00;
    GPIO_PORTF_DIR_R |= 0x00000001;
    
    //Disable other functions by settinng all bits except 7th bit to 0;
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    
    // enable the 0th bit in the digital enable regester;
    GPIO_PORTF_DEN_R &= 0xFFFFFF00;
    GPIO_PORTF_DEN_R |= 0x00000001;
    /*
    // Turn on the LED;
    GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
    GPIO_PORTF_DATA_R |= 0x00000001;
    */
}

// PRE:
//      myPtr - a pointer to the current position of the display cursor;
// writes to the display the main menu;
//
// POST:
//      the main menu is written to the display;
void display_menu(int* myPtr) {
    char* line1="BEEF";
    char* line2="PORK";
    
    *myPtr=0;
    // Write the 1st line;
    display_string(line1);
    *myPtr=strlen(line1)-1;
    
    // move the cursor to the beginning of the next line;
    volatile unsigned long i = 0;
    for (i = 0; i < 40 - strlen(line1); i++) {
        cd_shift(0x01);
    }
    
    *myPtr = 40;
    
    // Write the 2nd line;
    display_string(line2);
    *myPtr=40+strlen(line2);
}




///////////////////////////////////////////////////////////////
void timer0Initialize() {
	CPUcpsid();
        // clock the timer
	SYSCTL_RCGC1_R |= 0x00010000;
	for (int i = 0; i < 500; i++);
	// disable the timer A
	TIMER0_CTL_R &= 0xFFFFFFFE;
	// set timer to 32-bit configuration
	TIMER0_CFG_R &= 0x0;
	// set timer to periodic mode
	TIMER0_TAMR_R |= 0x00000002;
	// set timer interval to largest possible value
	TIMER0_TAILR_R = 0x0000FFFF;
	// enable time out interrupts
	TIMER0_IMR_R |= 0x00000001;
	// start the timer
	
	// enable controller interrupts coming from the timer
	NVIC_EN0_R |= 0x00080000;
	CPUcpsie();
        TIMER0_CTL_R |= 0x00000001;
}

void Timer0IntHandler() {
  
  
    // clear timer0 interrupt regesters
    TIMER0_ICR_R |= 0x00000001;
    CPUcpsid();
	
    if (flag) {
    	curKey = getKey();
    } else {
		if((is_a_key(curKey)) && (curKey == getKey())){
			if(curKey != prevKey){
				prevKey = curKey;
				//processKey(curKey);
                                for (int i = 0; i < curKey; i++) {
                                    GPIO_PORTF_DATA_R |= 0x00000001;
                                    delay(1000);
                                    GPIO_PORTF_DATA_R &= 0xFFFFFFFE;
                                    delay(1000);
                                }
			}
		} else {
			prevKey = 0;
		}
    }
	flag = !flag;
    CPUcpsie();
  
  /*
   TIMER0_ICR_R |= 0x01;
    if ((GPIO_PORTF_DATA_R &= 0x00000001) > 0) {
    	GPIO_PORTF_DATA_R &= 0xFFFFFFFE;
    } else {
    	GPIO_PORTF_DATA_R |= 0x00000001;
    }*/
}

//  software delay;
void delay(int aDelay) {
    volatile unsigned long i = 0;

    volatile unsigned int j = 0;
    
    for (i = aDelay; i > 0; i--) {
        for (j = 0; j < 100; j++);
    }
}