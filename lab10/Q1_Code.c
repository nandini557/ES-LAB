// Write a c program to display the digital value representing the difference in analog voltages at ADC channel 4 and channel 5 on LCD.

#include <LPC17xx.h>
#include <stdio.h>

#define RS 0x01  // P0.0
#define RW 0x02  // P0.1
#define EN 0x04  // P0.2

void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_display(char *msg);
void delay_ms(unsigned int ms);
unsigned short read_adc(unsigned int channel);

int main(void) {
    unsigned short adc4_val, adc5_val, diff;
    char msg[16];

    // LCD & ADC initialization
    lcd_init();

    // Configure P1.30 (AD4) and P1.31 (AD5)
    LPC_PINCON->PINSEL3 |= (3 << 28); // P1.30 = AD4
    LPC_PINCON->PINSEL3 |= (3 << 30); // P1.31 = AD5

    // Enable ADC peripheral power
    LPC_SC->PCONP |= (1 << 12);       // Power up ADC
    LPC_ADC->ADCR = (1 << 21);        // ADC enable (PDN = 1)

    while (1) {
        adc4_val = read_adc(4);       // Read from AD4 (P1.30)
        adc5_val = read_adc(5);       // Read from AD5 (P1.31)

        // Calculate digital difference
        diff = (adc4_val > adc5_val) ? (adc4_val - adc5_val) : (adc5_val - adc4_val);

        lcd_cmd(0x01);                // Clear LCD
        sprintf(msg, "Diff: %u", diff);
        lcd_display(msg);

        delay_ms(1000);               // 1 sec delay
    }
}
void lcd_init() {
    LPC_GPIO0->FIODIR |= 0xFF; // Set P0.0–P0.7 as output
    delay_ms(20);
    lcd_cmd(0x28); // 4-bit, 2 lines
    lcd_cmd(0x0C); // Display ON, Cursor OFF
    lcd_cmd(0x06); // Auto increment
    lcd_cmd(0x01); // Clear
}

void lcd_cmd(unsigned char cmd) {
    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = (cmd & 0xF0);      // Upper nibble
    LPC_GPIO0->FIOCLR = RS | RW;
    LPC_GPIO0->FIOSET = EN; delay_ms(2); LPC_GPIO0->FIOCLR = EN;

    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = ((cmd << 4) & 0xF0); // Lower nibble
    LPC_GPIO0->FIOCLR = RS | RW;
    LPC_GPIO0->FIOSET = EN; delay_ms(2); LPC_GPIO0->FIOCLR = EN;
}

void lcd_data(unsigned char data) {
    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = (data & 0xF0);     // Upper nibble
    LPC_GPIO0->FIOSET = RS;
    LPC_GPIO0->FIOCLR = RW;
    LPC_GPIO0->FIOSET = EN; delay_ms(2); LPC_GPIO0->FIOCLR = EN;

    LPC_GPIO0->FIOCLR = 0xFF;
    LPC_GPIO0->FIOSET = ((data << 4) & 0xF0); // Lower nibble
    LPC_GPIO0->FIOSET = RS;
    LPC_GPIO0->FIOCLR = RW;
    LPC_GPIO0->FIOSET = EN; delay_ms(2); LPC_GPIO0->FIOCLR = EN;
}

void lcd_display(char *msg) {
    while (*msg)
        lcd_data(*msg++);
}

void delay_ms(unsigned int ms) {
    for (unsigned int i = 0; i < ms * 6000; i++);  // Approximate delay
}

unsigned short read_adc(unsigned int channel) {
    LPC_ADC->ADCR = (1 << channel) | (1 << 21);  // Select channel, enable ADC
    LPC_ADC->ADCR |= (1 << 24);                 // Start conversion

    while (!(LPC_ADC->ADGDR & (1 << 31)));      // Wait until DONE
    return (LPC_ADC->ADGDR >> 4) & 0xFFF;       // 12-bit result
}
