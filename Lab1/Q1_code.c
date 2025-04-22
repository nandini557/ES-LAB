// Write an embedded C program to blink an LED.

#include <LPC17xx.h>  // Device header

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);  // Rough 1ms delay
}

int main(void) {
    // Configure P0.22 as output (LED connected here)
    LPC_GPIO0->FIODIR |= (1 << 22); // Set bit 22 of PORT0 as output

    while (1) {
        LPC_GPIO0->FIOSET = (1 << 22); // Set P0.22 HIGH (LED ON)
        delay_ms(500);                 // Delay 500 ms

        LPC_GPIO0->FIOCLR = (1 << 22); // Set P0.22 LOW (LED OFF)
        delay_ms(500);                 // Delay 500 ms
    }
}

