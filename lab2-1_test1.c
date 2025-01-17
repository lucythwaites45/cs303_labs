/*
Hopefully using alt_alarm_start and alt_alarm_stop in our
buttomn ISR is safe :')
*/

#include <stdio.h>
#include "system.h"
#include <sys/alt_alarm.h>
#include <altera_avalon_pio_regs.h>

volatile int counter = 0;
volatile int key1_pressed = 0;
volatile int counter_changed = 0;
volatile int timer_running = 0;
alt_alarm timer;

alt_u32 timer_isr(void* context) {
    if (key1_pressed) {  // KEY1 held
        counter++;
        counter_changed = 1;
    }
    return 500;
}

void button_isr(void* context, alt_u32 id) {
    int button_pressed = IORD_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE);
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0); // Clear edge capture reg

    if (button_pressed & 0x01) { // KEY0 pressed
        counter++;
        counter_changed = 1;
    }

    if (button_pressed & 0x02) { // KEY1 pressed
        key1_pressed = 1;
        if (!timer_running) { // Restart timer if not running
        	alt_alarm_start(&timer, 500, timer_isr, NULL);
        	timer_running = 1;
        }
    } else { // KEY1 released
        key1_pressed = 0;
        if (timer_running) { // Stop timer if running
        	alt_alarm_stop(&timer);
        	timer_running = 0;
        }
    }
}

int main() {
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEYS_BASE, 0); // Clear edge capture reg
    IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEYS_BASE, 0x7); // Enable interrupts for all buttons
    alt_irq_register(KEYS_IRQ, NULL, button_isr); // Register ISRs

    while(1) {
        if (counter_changed) {
            printf("Counter: %d\n", counter);
            counter_changed = 0;
        }
        // usleep(1000000); // Optional delay
    }
}
