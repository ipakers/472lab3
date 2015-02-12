extern void delay(int aDelay);
void display_init();
void display_sequence(uint32_t data, uint32_t control);
void display_write(char input, int rs);
void display_string(char* str);
