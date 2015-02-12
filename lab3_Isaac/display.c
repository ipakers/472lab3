#include "inc/lm3s8962.h"
#include "stdio.h"
#include <stdint.h>
#include "display.h"

void display_init() {
  // SET UP LOCAL REGISTERS  
  
  // clock ports B and D
    SYSCTL_RCGC2_R |= 0x0000000A;
    
    // Set 7 out of 8 pins in port B to be outputs
    GPIO_PORTB_DIR_R |= 0x0000007F;
    // Disable other functions of port B by settinng all bits except 7th bit to 0
    GPIO_PORTB_AFSEL_R &= 0xFFFFFF80;
    // Enable 7 out of 8 pins in port B 
    GPIO_PORTB_DEN_R |= 0x0000007F;
    
    // Set top 3 pins in port D to be outputs
    GPIO_PORTD_DIR_R |= 0x000000E0;
    // Disable other functions of port D by settinng all bits except 7th bit to 0
    GPIO_PORTD_AFSEL_R &= 0xFFFFFF80;
    // Enable top 3 pins in port D
    GPIO_PORTD_DEN_R |= 0x000000E0;
    
    // INITIALIZATION
    display_sequence(0x00000000, 0x0000030);
    
    display_sequence(0x00000000, 0x0000030);

    display_sequence(0x00000000, 0x0000030);
    
    // Function Set
    display_sequence(0x00000000, 0x0000038);
    // display OFF
    display_sequence(0x00000000, 0x0000008);
    // clear display
    display_sequence(0x00000000, 0x0000001);
    // entry mode set
    display_sequence(0x00000000, 0x0000004);
    // display ON
    display_sequence(0x00000000, 0x000000F);
}

void display_sequence(uint32_t control, uint32_t data) {
    delay(500);  
    // clear last data
    GPIO_PORTB_DATA_R &= 0xFFFFFF00;
    // assign new data
    GPIO_PORTB_DATA_R |= data;
    
    // clear last control
    GPIO_PORTD_DATA_R &= 0xFFFFFF1F;
    // assign new control
    GPIO_PORTD_DATA_R |= control;
    delay(10);
    
    // enable
    GPIO_PORTD_DATA_R |= 0x00000020;
    delay(100);
    
    // disable
    GPIO_PORTD_DATA_R &= 0xFFFFFFDF;
    delay(10);
}

// input a character and an integer 0 or 1. 1 for write, 0 for read
// output that character on the display
void display_write(char input, int rs) {
   delay(250);  
   uint32_t data = input;
   
   if(rs == 1)
   {
   //make RS high
   GPIO_PORTD_DATA_R |= 0x00000080;
   delay(10);
   
   // clear last data
   GPIO_PORTB_DATA_R &= 0xFFFFFF00;
   // assign new data
   GPIO_PORTB_DATA_R |= data;
   
   // enable
   GPIO_PORTD_DATA_R |= 0x00000020;
   delay(100);
   
   // disable
   GPIO_PORTD_DATA_R &= 0xFFFFFFDF;
   delay(10);
   
   //make RS low
   GPIO_PORTD_DATA_R &= 0xFFFFFF7F;
   delay(10);
   }
}

// input a string
// output the string on the display
void display_string(char* str)
{
  display_sequence(0x00000000, 0x0000006);      // set cursor on entry mode (auto move cursor to the right)
  int i=0;
  while(str[i] != '\0')
  { 
    display_write(str[i],1);    // write the character at that ith position
    i=i+1;
  }
}

