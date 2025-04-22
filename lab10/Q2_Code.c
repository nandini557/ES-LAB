/* Write a program to set the following intensity levels to the LED connected to PWM output. Use ROW-0 of keyboard for intensity variation 
Intensity level Key pressed
10% 0
25% 1
50% 2
75% 3 */

#include <LPC17xx.h>

// Function Prototypes
void keypad_init(void);
char get_key(void);
void pwm_init(void);
void set_intensity(int percent);
void delay_ms(unsigned int ms);

int main(void) {
    char key;

    keypad_init();     // Initialize keypad pins
    pwm_init();        // Initialize PWM

    while (1) {
        key = get_key();  // Read key from Row-0
        switch (key) {
            case '0':
                set_intensity(10);
                break;
            case '1':
                set_intensity(25);
                break;
            case '2':
                set_intensity(50);
                break;
            case '3':
                set_intensity(75);
                break;
        }
        delay_ms(300); // Debounce delay
    }
}

void pwm_init(void) {
    LPC_PINCON->PINSEL4 |= (1 << 4);    // P2.1 → PWM1.2
    LPC_SC->PCONP |= (1 << 6);          // Power up PWM1
    LPC_PWM1->PR = 0;                   // No prescale
    LPC_PWM1->MR0 = 1000;               // Set PWM period (1ms)
    LPC_PWM1->MR2 = 100;                // Default 10% duty cycle
    LPC_PWM1->MCR = (1 << 1);           // Reset on MR0
    LPC_PWM1->LER = (1 << 0) | (1 << 2);// Load MR0 and MR2
    LPC_PWM1->PCR = (1 << 10);          // Enable PWM1.2 output
    LPC_PWM1->TCR = (1 << 0) | (1 << 3);// Enable counter and PWM
}
void set_intensity(int percent) {
    LPC_PWM1->MR2 = (percent * LPC_PWM1->MR0) / 100;
    LPC_PWM1->LER = (1 << 2); // Update MR2
}
void keypad_init(void) {
    LPC_GPIO2->FIODIR |= 0x0F;  // Set P2.0–P2.3 (Rows) as output
    LPC_GPIO2->FIODIR &= ~(0xF0); // Set P2.4–P2.7 (Cols) as input
}

char get_key(void) {
    LPC_GPIO2->FIOPIN = 0x0E; // Activate Row 0 (P2.0 = 0)
    delay_ms(10);
    if (!(LPC_GPIO2->FIOPIN & (1 << 4))) return '0'; // Col0
    if (!(LPC_GPIO2->FIOPIN & (1 << 5))) return '1'; // Col1
    if (!(LPC_GPIO2->FIOPIN & (1 << 6))) return '2'; // Col2
    if (!(LPC_GPIO2->FIOPIN & (1 << 7))) return '3'; // Col3
    return 0xFF;
}
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}
