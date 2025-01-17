#include <stdio.h>
#include "system.h"
#include <sys/alt_alarm.h>

#include "altera_avalon_pio_regs.h"

volatile int counter = 0;


alt_u32 timer_isr_function(void* context){
	int *timeCount = (int*) context;
	(*timeCount)++;
	printf("time:%d\n", *timeCount);
	return 500; // the next time out will be 1000 milli-seconds
}

// first we write our interrupt function
void button_interrupts_function(void* context, alt_u32 id){
	int* temp = (int*) context; // need to cast the context first before using it
	(*temp) = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	// clear the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);

	if (*temp == 1){
		counter++;
		printf("counter:%d\n", counter);
	}

}

int main(){

	alt_alarm timer;
	int timeCountMain = 0;
	void* timerContext = (void*) &timeCountMain;
	//alt_alarm_start(&timer, 500, timer_isr_function, timerContext);

	int buttonValue = 1;
	void* context_going_to_be_passed = (void*) &buttonValue; // cast before passing to ISR

	// clears the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);

	// enable interrupts for all buttons
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x7);
	// register the ISR
	alt_irq_register(KEYS_IRQ,context_going_to_be_passed,button_interrupts_function);



	while(1){
		if (buttonValue == 2){
			alt_alarm_start(&timer, 500, timer_isr_function, timerContext);
		}else{
			alt_alarm_stop(&timer);
		}


		if ( timeCountMain == 10)
			{
			alt_alarm_stop(&timer);
			usleep(1000000);
			alt_alarm_start(&timer, 500, timer_isr_function, timerContext);
			usleep(1500000);
			}


	}
	return 0;
}
