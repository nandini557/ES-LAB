// Write a C program to display 8-bit binary up counter on the LEDs.

#include <LPC17xx.h>  // Device header for LPC1768

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);  // Rough 1ms delay
}

int main(void) {
    unsigned char counter = 0;

    // Set P1.0 to P1.7 as output for LEDs
    LPC_GPIO1->FIODIR |= 0xFF;  // 0xFF = 0b11111111 → lower 8 bits

    while (1) {
        LPC_GPIO1->FIOCLR = 0xFF;             // Clear all 8 pins first
        LPC_GPIO1->FIOSET = counter;          // Set bits based on counter value
        delay_ms(500);                        // Wait 0.5 seconds
        counter++;                            // Increment the 8-bit counter (rolls over after 255)
    }
}

