void Timer0IntHandler(void)
{

    //turn on LED
    //
    
    uint32_t read;
     
    //alternate between request and read
    //
    ADC0_PSSI_R |= 0x00000001;//Request
    read = ADC0_SSFIFO0_R;
    
    //if read ADC >= 512
    //turn on LED
	if (read >= 0x00000200) {
		GPIO_PORTF_DATA_R &= 0xFFFFFFFC;
		GPIO_PORTF_DATA_R |= 0x00000001;
	}
    
	//pass it to change TIMER1_TAILR to change how often
	//Timer1EventHandler gets called    
	
	

    
    //clear interrupt
    TIMER0_ICR
    
}

//to request a sample
void ADC_init(void){
	SYSCTL_RCGC0_R |= 00010000;
	ADC0_ACTSS_R |= 00000001;
}

void Timer1IntHandler(void)
{
    //turn on LED
    //

    //clear interrupt
    TIMER1_ICR

}

void timer1_init(uint32_t TAILRValue){
    SYSCTL_RCGC1_R |= 0x020;    	//bit 17 for timer 1
	////not sure if i need to modify CTL didnt find the reg in datasheet
    TIMER1_CTL_R &= 0xEFE;                  // disable timerA and timerB
	
	TIMER1_CFG_R |= TIMER_CFG_16_BIT;
	TIMER1_TAMR_R |=  TIMER_TAMR_TAMR_PERIOD;
	
	////not sure how to pass the given value to it (how to filter ou)
    TIMER1_TAILR_R |= 0x400;                // set timer to count down from 1024
    TIMER1_TAILR_R |= 0xFFFFFC00;
	
	
	TIMER1_IMR_R |= 0x001;                  // enables interrupts
    TIMER1_CTL_R |= 0x001;
	
	//////////NVIC_EN0 |= 0x001;
	CPUcpsie();
}

