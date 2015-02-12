//*****************************************************************************
//
// Table of Contents:
// delay(unsigned long aValue) - pauses the execution of the program. 
//     aValue is the number of clock cycles that the program pauses. The 
//     software needs to pause in order for the characters to become visible.
//
//*****************************************************************************
#include "inc/lm3s8962.h"
#include "stdio.h"
#include <stdint.h>
#include "keypad.h"

#define TRUE 1
#define FALSE 0

extern uint32_t prevKey;

// initialize the keypad
void key_init() {
    // Note that select is in port F, and left, right, up, down, is in port E
    // Enable the GPIO port that is used for the on-board LED.
  
    // clock the GPIO port E and F
    SYSCTL_RCGC2_R |= 0x00000030;
    
    //  Set all pins in port E to be inputs
    GPIO_PORTE_DIR_R &= 0xFFFFFF00;
    
    //Disable other functions of port F and E by settinng all bits except 7th bit to 0
    GPIO_PORTF_AFSEL_R &= 0xFFFFFF80;
    GPIO_PORTE_AFSEL_R &= 0xFFFFFF80;
    
    // set the pull up select regester
    GPIO_PORTF_PUR_R |= 0x00000002;
    GPIO_PORTE_PUR_R |= 0x0000000F;

    // disable all port E pins, then enable bottom for pins
    GPIO_PORTE_DEN_R &= 0xFFFFFFF0;
    GPIO_PORTE_DEN_R |= 0x0000000F;

    // enable the second to last pin of port F
    GPIO_PORTF_DEN_R |= 0x00000002;
}

int is_a_key(int curKey) {
    if (curKey > 0) {
        return TRUE;
    } else if (curKey > 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int getKey() {
    uint32_t myVar = (~(GPIO_PORTE_DATA_R)) & 0x0000000F;
    if (((~(GPIO_PORTF_DATA_R)) & 0x00000002) > 0) {
        return 5;
    } else if (myVar == 0x00000001) { // up
        return 1;
    } else if (myVar == 0x00000002) { // down
        return 2;
    } else if (myVar == 0x00000004) { // left
        return 3;
    } else if (myVar == 0x00000008) { // right
        return 4;
    } else  {
        return 0;
    }
}
extern int curKey;
int keyMaster() {
  if (is_a_key(curKey) && debounce()) {
    if (fresh_key()) {
      prevKey = getKey();
      return prevKey;
    } else {
      return 0;
    }
  } else {
      prevKey = 0;
      return 0;
  }
}

int debounce() {
  volatile unsigned long i = 0;
  int key = getKey();
  for (i = 0; i < 1500; i++) {
    if (key != getKey() ){
      return FALSE;
    }
  }
  return TRUE;
}

int fresh_key() {
  return getKey() != prevKey;
}