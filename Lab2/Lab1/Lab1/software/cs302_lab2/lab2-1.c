#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <stdio.h>
#include "SCC_Charts.h"


int main(){

	TickData data;

	//int buttonValue = 1;
	//void* context_going_to_be_passed = (void*) &buttonValue;
	// Create the struct


	// Initialise
	reset(&data);


	int buttonValue = 1;

	while(1)
	{
		// Fetch button inputs
		// A is Key 2, B is Key 1, R is Key 0
		// Remember that keys are active low


		buttonValue = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
		if  (buttonValue == 1){
			data.R = 1;

		}else if (buttonValue == 2){
			data.B = 1;
		}else if (buttonValue == 3){
			data.A = 1;
		}

		// Do a tick!
		tick(&data);
				// Output O to Red LED
		if (data.O == 1){
				IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0xFF);
			}





	}
	return 0;
}