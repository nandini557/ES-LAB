// Write a C program for 4 digit BCD up/down counter on seven segment using a switch and timer with a delay of 1-second between each count.

#include <LPC17xx.h>

const unsigned char segment_code[10] = {0x3F, 0x06, 0x5B, 0x4F,
                                        0x66, 0x6D, 0x7D, 0x07,
                                        0x7F, 0x6F};

unsigned int count = 0;

void delay_us(unsigned int us) {
    unsigned int i, j;
    for (i = 0; i < us; i++)
        for (j = 0; j < 10; j++);
}

void display_digits(unsigned int number) {
    unsigned int digits[4];
    int i;

    digits[0] = number % 10;
    digits[1] = (number / 10) % 10;
    digits[2] = (number / 100) % 10;
    digits[3] = (number / 1000) % 10;

    for (i = 0; i < 4; i++) {
        LPC_GPIO1->FIOCLR = (0xF << 23);              // Turn off all digits
        LPC_GPIO0->FIOCLR = 0x7F;                     // Clear segment pins

        LPC_GPIO0->FIOSET = segment_code[digits[i]];  // Display digit
        LPC_GPIO1->FIOSET = (1 << (23 + i));          // Enable one digit

        delay_us(1000);  // Short delay for multiplexing
    }
}

void timer0_init(void) {
    LPC_TIM0->PR = 29999;      // 30,000 prescaler = 1 ms
    LPC_TIM0->MR0 = 1000;      // 1000 ms = 1 second
    LPC_TIM0->MCR = 3;         // Interrupt & Reset on MR0
    LPC_TIM0->TCR = 1;         // Enable timer
}

void delay_1s(void) {
    LPC_TIM0->TCR = 1;  // Start timer
    while (!(LPC_TIM0->IR & 0x01));  // Wait for match
    LPC_TIM0->IR = 1;    // Clear interrupt flag
}

int main(void) {
    // Configure segment pins P0.0–P0.6 as output
    LPC_GPIO0->FIODIR |= 0x7F;

    // Configure digit control P1.23–P1.26 as output
    LPC_GPIO1->FIODIR |= (0xF << 23);

    // Configure switch P2.10 as input
    LPC_PINCON->PINSEL4 &= ~(3 << 20);    // P2.10 = GPIO
    LPC_GPIO2->FIODIR &= ~(1 << 10);      // Input

    timer0_init();  // Initialize Timer0 for 1 sec

    while (1) {
        // Display the number continuously (multiplexing)
        for (int i = 0; i < 50; i++)  // approx 50 ms of multiplexing
            display_digits(count);

        delay_1s();  // 1 second delay using Timer0

        // Check switch state to count up or down
        if (LPC_GPIO2->FIOPIN & (1 << 10))
            count = (count + 1) % 10000;   // Up counter (rolls over after 9999)
        else
            count = (count == 0) ? 9999 : count - 1; // Down counter
    }
}
