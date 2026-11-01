#include <io.h>
#include <alcd.h>
#include <delay.h>

#define BT1 PINB.2
#define BT2 PINB.3
#define BT3 PINB.0

#define RELAY1 PORTA.0
#define RELAY2 PORTA.1
#define RELAY3 PORTA.2

void main(void) {
    DDRB = 0x00;
    PORTB = 0x0D;

    DDRA = 0xFF;
    PORTA = 0x00;

    DDRD = 0x80;
    PORTD = 0x80;

    lcd_init(16);
    lcd_clear();
    
    lcd_gotoxy(0, 0);
    lcd_puts("Chon nut bam...");

    while (1) {
        if (BT1 == 0) {
            delay_ms(50);
            if (BT1 == 0) {
                RELAY1 = 1;
                RELAY2 = 0;
                RELAY3 = 0;

                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts("Nut bam: BT-1");
                lcd_gotoxy(0, 1);
                lcd_puts("Role 1: DONG");

                while (BT1 == 0);
            }
        }

        if (BT2 == 0) {
            delay_ms(50);
            if (BT2 == 0) {
                RELAY1 = 0;
                RELAY2 = 1;
                RELAY3 = 0;

                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts("Nut bam: BT-2");
                lcd_gotoxy(0, 1);
                lcd_puts("Role 2: DONG");

                while (BT2 == 0);
            }
        }

        if (BT3 == 0) {
            delay_ms(50);
            if (BT3 == 0) {
                RELAY1 = 0;
                RELAY2 = 0;
                RELAY3 = 1;

                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts("Nut bam: BT-3");
                lcd_gotoxy(0, 1);
                lcd_puts("Role 3: DONG");

                while (BT3 == 0);
            }
        }
    }
}