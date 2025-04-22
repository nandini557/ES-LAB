// Write a program to display 4-digit binary ring counter on the multiplexed seven segment display.

#include <LPC17xx.h>

const unsigned char segment_code[2] = {
    0x3F, // 0
    0x06  // 1
};

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}

void display_binary_ring(unsigned char value) {
    for (int i = 0; i < 4; i++) {
        // Turn off all digits
        LPC_GPIO1->FIOCLR = (0xF << 23);  // Clear P1.23 - P1.26
        LPC_GPIO0->FIOCLR = 0x7F;         // Clear segments P0.0–P0.6

        // Get bit value at position i
        unsigned char bit = (value >> i) & 0x01;

        // Set segment data for 0 or 1
        LPC_GPIO0->FIOSET = segment_code[bit];

        // Enable corresponding digit
        LPC_GPIO1->FIOSET = (1 << (23 + i));

        delay_ms(2);  // short delay for multiplexing
    }
}

int main(void) {
    unsigned char ring = 0x01;  // Initial value: 0001

    // Set segment pins P0.0–P0.6 as output
    LPC_GPIO0->FIODIR |= 0x7F;

    // Set digit select pins P1.23–P1.26 as output
    LPC_GPIO1->FIODIR |= (0xF << 23);

    while (1) {
        // Show current ring counter value on display (repeat for persistence)
        for (int i = 0; i < 200; i++)  // 200×2ms = ~400ms
            display_binary_ring(ring);

        // Rotate ring counter left
        ring = (ring << 1);
        if (ring == 0)
            ring = 0x01;  // Reset back to 0001 after 1000
    }
}
