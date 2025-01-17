/*
 * "Hello World" example.
 *
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example
 * designs. It runs with or without the MicroC/OS-II RTOS and requires a STDOUT
 * device in your system's hardware.
 * The memory footprint of this hosted application is ~69 kbytes by default
 * using the standard reference design.
 *
 * For a reduced footprint version of this template, and an explanation of how
 * to reduce the memory footprint for a given application, see the
 * "small_hello_world" template.
 *
 */


#include <system.h> // to use the symbolic names
#include <altera_avalon_pio_regs.h> // to use PIO functions
#include <alt_types.h> // alt_u32 is a kind of alt_types
#include <sys/alt_irq.h> // to register interrupts
#include "sys/alt_alarm.h"
#include <stdio.h>


volatile unsigned int counterPrevious = 0;
volatile unsigned int counter = 0;
// first we write our interrupt function
void button_interrupts_function(void* context, alt_u32 id)
{
	int* temp = (int*) context; // need to cast the context first before using it
	(*temp) = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
	// clear the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);
	if (*temp == 1){
		counter ++;
		printf("button: %i\n", counter);
	}


}

alt_u32 timer_isr_function(void* context)
{
	int *timeCount = (int*) context;
	(*timeCount)++;
	printf("time:%d\n", *timeCount);
	return 500; // the next time out will be 1000 milli-seconds
}

int main(void)
{

	int buttonValue = 1;
	FILE *lcd;
	void* context_going_to_be_passed = (void*) &buttonValue; // cast before


	//passing to ISR
// clears the edge capture register
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0);
// enable interrupts for all buttons
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x7);
// register the ISR
	alt_irq_register(KEYS_IRQ,context_going_to_be_passed,
			button_interrupts_function);

	alt_alarm timer;
	int timeCountMain = 0;

	void* timerContext = (void*) &timeCountMain;

	unsigned int uiButtonsValue = 0;
	unsigned int uiButtonsValuePrevious = 0;

	// start the timer, with timeout of 1000 milli-seconds

	while(1){


		int value = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
		int* temp = (int*) context_going_to_be_passed;

		//alt_alarm_start(&timer, 500, timer_isr_function, timerContext);

		/*if ( timeCountMain == 10)
		{
			alt_alarm_stop(&timer);
			usleep(1000000);
			alt_alarm_start(&timer, 1000, timer_isr_function, timerContext);
			usleep(1500000);
		}*/

		fprintf(lcd, "HELLO WORLD!!\n");
	}; // need this to keep the program alive
	fclose(lcd);
	return 0;
}

/*int main()
{
    unsigned int uiSwitchValue = 0;
    unsigned int uiButtonsValue = 0;
    unsigned int uiButtonsValuePrevious = 0;
    FILE *lcd;

    printf("Hello from Nios II!\n");
    IOWR_ALTERA_AVALON_PIO_DATA(LEDS_GREEN_BASE, 0xaa);

    lcd = fopen(LCD_NAME, "w");
    while (1)
    {
        uiSwitchValue = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
        IOWR_ALTERA_AVALON_PIO_DATA(LEDS_RED_BASE, uiSwitchValue);

        uiButtonsValuePrevious = uiButtonsValue;
        uiButtonsValue = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);

        if (lcd != NULL)
        {
            if (uiButtonsValuePrevious != uiButtonsValue)
            {
                #define ESC 27
                #define CLEAR_LCD_STRING "[2J"

                fprintf(lcd, "%c%s", ESC, CLEAR_LCD_STRING);
                fprintf(lcd, "BUTTON VALUE: %d\n", uiButtonsValue);
                fprintf(lcd, "HELLO WORLD BY ALTERA DAWGS!!!      \n");
            }
        }
    }

    fclose(lcd);
    return 0;
}*/

