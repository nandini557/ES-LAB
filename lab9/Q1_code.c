/* Write a program to input an expression of the type A operator B =, from the keyboard, where A and B are the single digit BCD numbers and operator may be + or
- .Display the result on the LCD.*/

#include <LPC17xx.h>
#include <stdio.h>

// LCD Control pins
#define RS (1 << 22)
#define RW (1 << 23)
#define EN (1 << 24)

// Function Prototypes
void lcd_init(void);
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_display(char *str);
void delay_ms(unsigned int ms);
char get_key(void);
void keypad_init(void);

int main(void) {
    char A = 0, B = 0, op = 0, key;
    int result = 0;
    char result_str[16];

    SystemInit();
    keypad_init();
    lcd_init();
    lcd_display("Expr: ");

    // Read A
    while (1) {
        key = get_key();
        if (key >= '0' && key <= '9') {
            A = key - '0';
            lcd_data(key);
            break;
        }
    }

    // Read Operator
    while (1) {
        key = get_key();
        if (key == '+' || key == '-') {
            op = key;
            lcd_data(key);
            break;
        }
    }

    // Read B
    while (1) {
        key = get_key();
        if (key >= '0' && key <= '9') {
            B = key - '0';
            lcd_data(key);
            break;
        }
    }

    // Wait for '='
    while (1) {
        key = get_key();
        if (key == '=') {
            lcd_data('=');
            break;
        }
    }

    // Calculate result
    result = (op == '+') ? (A + B) : (A - B);

    // Display result
    sprintf(result_str, "%d", result);
    lcd_display(result_str);

    while (1);
}
void lcd_init(void) {
    LPC_PINCON->PINSEL3 &= 0xFFFF00FF; // P1.23–P1.16 for data
    LPC_GPIO1->FIODIR |= 0x00FF0000;   // P1.16–P1.23 as output
    LPC_GPIO0->FIODIR |= RS | RW | EN; // RS, RW, EN as output

    delay_ms(20);
    lcd_cmd(0x38); // 2-line, 8-bit
    lcd_cmd(0x0C); // Display ON
    lcd_cmd(0x06); // Entry mode
    lcd_cmd(0x01); // Clear screen
}

void lcd_cmd(unsigned char cmd) {
    LPC_GPIO1->FIOPIN = (cmd << 16);
    LPC_GPIO0->FIOCLR = RS;
    LPC_GPIO0->FIOCLR = RW;
    LPC_GPIO0->FIOSET = EN;
    delay_ms(2);
    LPC_GPIO0->FIOCLR = EN;
}

void lcd_data(unsigned char data) {
    LPC_GPIO1->FIOPIN = (data << 16);
    LPC_GPIO0->FIOSET = RS;
    LPC_GPIO0->FIOCLR = RW;
    LPC_GPIO0->FIOSET = EN;
    delay_ms(2);
    LPC_GPIO0->FIOCLR = EN;
}

void lcd_display(char *str) {
    while (*str) {
        lcd_data(*str++);
    }
}
void keypad_init(void) {
    LPC_GPIO2->FIODIR |= 0x0F;     // Rows: P2.0–P2.3 = output
    LPC_GPIO2->FIODIR &= ~(0xF0);  // Cols: P2.4–P2.7 = input
}

char get_key(void) {
    char key_map[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    for (int row = 0; row < 4; row++) {
        LPC_GPIO2->FIOPIN = ~(1 << row); // Activate one row at a time
        delay_ms(10);

        for (int col = 0; col < 4; col++) {
            if (!(LPC_GPIO2->FIOPIN & (1 << (4 + col)))) {
                while (!(LPC_GPIO2->FIOPIN & (1 << (4 + col)))); // Wait for release
                return key_map[row][col];
            }
        }
    }
    return 0xFF; // No key pressed
}
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 6000; j++);
}

