/* Write a C program to read a key and display an 8-bit up/down counter on the LEDs. Hint: Use key SW2(if SW2=1, up counter else down counter), which is available
at CNB1 pin 7. Connect CNB1 to any controller connector like CNB, CNC etc. Configure corresponding port pin as GPIO using corresponding PINSEL register
and input pin using corresponding FIODIR register. */

#include <LPC17xx.h>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}

int main(void) {
    unsigned char counter = 0;

    // STEP 1: Configure P2.10 as GPIO
    LPC_PINCON->PINSEL4 &= ~(3 << 20);  // Bits 20-21 for P2.10: 00 = GPIO

    // STEP 2: Set P2.10 as input (SW2)
    LPC_GPIO2->FIODIR &= ~(1 << 10);    // Clear bit 10 → Input

    // STEP 3: Set P1.0 to P1.7 as output (LEDs)
    LPC_GPIO1->FIODIR |= 0xFF;          // Lower 8 bits of P1 = output

    while (1) {
        // STEP 4: Read SW2 status
        if ((LPC_GPIO2->FIOPIN & (1 << 10))) {
            counter++;  // SW2 = 1 → Up counter
        } else {
            counter--;  // SW2 = 0 → Down counter
        }

        // STEP 5: Output counter value to LEDs
        LPC_GPIO1->FIOCLR = 0xFF;        // Clear all LEDs
        LPC_GPIO1->FIOSET = counter;     // Set LEDs based on counter

        delay_ms(500);  // Visible delay
    }
}

