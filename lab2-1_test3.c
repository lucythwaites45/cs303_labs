/*
CURRENT WORKING VERSION!! :D
*/

#include <stdio.h>
#include "system.h"
#include <sys/alt_alarm.h>
#include <altera_avalon_pio_regs.h>

volatile int counter = 0;
volatile int counter_changed = 0;
alt_alarm timer;

alt_u32 timer_isr(void* context) {
    counter++;
    counter_changed = 1;
    return 500;
}

int main() {
	// KEY0 AND KEY1 FUNCTIONALITY
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0); // Clear edge capture reg
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x7); // Enable interrupts for all buttons

    int timer_running = 0;
    int key1_pressed = 0;
    int key0_last_state = 1;

    // PRINTING TO LCD
    unsigned int uiSwitchValue = 0;
	unsigned int uiButtonsValue = 0;
	unsigned int uiButtonsValuePrevious = 0;
	FILE *lcd;
	lcd = fopen(LCD_NAME, "w");

    while(1) {
    	// KEY0 AND KEY1 FUNCTIONALITY
        int current_state = IORD_ALTERA_AVALON_PIO_DATA(KEYS_BASE);
        if ((current_state & 0x02) == 0) { // KEY1 pressed
            if (!key1_pressed) {
                key1_pressed = 1;
                if (!timer_running) {
                    alt_alarm_start(&timer, 500, timer_isr, NULL);
                    timer_running = 1;
                }
            }
        } else { // KEY1 released
            if (key1_pressed) {
                key1_pressed = 0;
                if (timer_running) {
                    alt_alarm_stop(&timer);
                    timer_running = 0;
                }
            }
        }

        if ((current_state & 0x01) == 0) { // KEY0 pressed
            if (key0_last_state == 1) {
                counter++;
                counter_changed = 1;
            }
            key0_last_state = 0;
        } else { // KEY0 released
            key0_last_state = 1;
        }

        if (counter_changed) {
            printf("Counter: %d\n", counter);
            counter_changed = 0;
        }


        // PRINTING TO LCD
		if (lcd != NULL) {
			#define ESC 27
			#define CLEAR_LCD_STRING "[2J"

			// fprintf(lcd, "%c%s", ESC, CLEAR_LCD_STRING); // Shows only one counter on LCD
			fprintf(lcd, "Counter: %d\n", counter);
		}
    }

    fclose(lcd);
	return 0;
}
