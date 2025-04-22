/* Write an embedded C program to turn ON the LED when the switch connected to P2.12 is pressed and turn OFF the LED when the switch is released.

#include <LPC17xx.h>  // Include LPC1768 definitions

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);  // Rough 1ms delay
}

int main(void) {
    // Set P0.22 as output (for LED)
    LPC_GPIO0->FIODIR |= (1 << 22);

    // Set P2.12 as input (for switch)
    LPC_GPIO2->FIODIR &= ~(1 << 12);

    while (1) {
        // Check if switch is pressed (active LOW)
        if ((LPC_GPIO2->FIOPIN & (1 << 12)) == 0) {
            LPC_GPIO0->FIOSET = (1 << 22);  // Turn ON LED
        } else {
            LPC_GPIO0->FIOCLR = (1 << 22);  // Turn OFF LED
        }
    }
}
