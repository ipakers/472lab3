#include "inc/lm3s8962.h"
#include "stdio.h"
#include <stdint.h>
#include "advanced_display.h"

// clears the display
void clear_display() {
  display_sequence(0x00000000, 0x0000001);
}

// inputs: 0 0 shift cursor left by 1 display unit
//         0 1 shift cursor right
//         1 0 shift display left
//         1 1 shift display right
// invalid inputs does nothing
void cd_shift(uint32_t shift) {
  if(shift == 0x00) {
    display_sequence(0x00000000, 0x00000010);
  } else if (shift == 0x01) {
    display_sequence(0x00000000, 0x00000014);
  } else if (shift == 0x02) {
    display_sequence(0x00000000, 0x00000018);
  } else if (shift == 0x03) {
    display_sequence(0x00000000, 0x0000001C);
  } else {
    return;
  }
}
