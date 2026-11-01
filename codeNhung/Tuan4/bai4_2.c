// Dieu khien LCD bang 3 nut nhan
// BT1 (PB2): chu "Hello-World" chay tu trai sang phai
// BT2 (PB3): chu chay tu phai sang trai
// BT3 (PB0): xoa man hinh

#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <string.h>

#define BT1 PINB.2     // nut dich phai
#define BT2 PINB.3     // nut dich trai
#define BT3 PINB.0     // nut xoa man hinh

void main(void) {
    char text[] = "Hello-World";   // 11 ky tu
    int i;

    // PORTB la input, bat pull-up cho PB0/PB2/PB3
    // Chua nhan = 1, nhan = 0
    DDRB  = 0x00;
    PORTB = 0x0D;

    // PD7 dieu khien den nen LCD
    DDRD  = 0x80;
    PORTD = 0x80;

    lcd_init(16);
    lcd_clear();

    while (1) {
        // BT1: chay chu sang phai (cot 0 -> 5)
        if (BT1 == 0) {
            delay_ms(50);                 // chong doi
            if (BT1 == 0) {
                for (i = 0; i <= 5; i++) {
                    lcd_clear();
                    lcd_gotoxy(i, 0);
                    lcd_puts(text);
                    delay_ms(200);        // toc do chay
                }
                while (BT1 == 0);         // cho nha nut
            }
        }

        // BT2: chay chu sang trai (cot 5 -> 0)
        if (BT2 == 0) {
            delay_ms(50);
            if (BT2 == 0) {
                for (i = 5; i >= 0; i--) {
                    lcd_clear();
                    lcd_gotoxy(i, 0);
                    lcd_puts(text);
                    delay_ms(200);
                }
                while (BT2 == 0);
            }
        }

        // BT3: xoa man hinh
        if (BT3 == 0) {
            delay_ms(50);
            if (BT3 == 0) {
                lcd_clear();
            }
            while (BT3 == 0);
        }
    }
}