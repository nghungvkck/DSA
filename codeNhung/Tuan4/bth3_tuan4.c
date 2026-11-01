#include <io.h>
#include <alcd.h>
#include <delay.h>

#define BT1 PINB.2

unsigned char page = 0;

void display_page(unsigned char p) {
    lcd_clear();
    switch (p) {
        case 0:
            lcd_gotoxy(0, 0);
            lcd_puts("3. Doc trang vb ");
            lcd_gotoxy(0, 1);
            lcd_puts("tren LCD 16x2...");
            break;
        case 1:
            lcd_gotoxy(0, 0);
            lcd_puts("khi bam BT-1 se ");
            lcd_gotoxy(0, 1);
            lcd_puts("lat tung trang  ");
            break;
        case 2:
            lcd_gotoxy(0, 0);
            lcd_puts("man hinh de doc ");
            lcd_gotoxy(0, 1);
            lcd_puts("noi dung tiep   ");
            break;
        case 3:
            lcd_gotoxy(0, 0);
            lcd_puts("theo. Het trang!");
            lcd_gotoxy(0, 1);
            lcd_puts("Nhan de ve dau  ");
            break;
    }
}

void main(void) {
    DDRB = 0x00;
    PORTB = 0x04;

    DDRD = 0x80;
    PORTD = 0x80;

    lcd_init(16);
    display_page(page);

    while (1) {
        if (BT1 == 0) {
            delay_ms(50);
            if (BT1 == 0) {
                page++;
                if (page > 3) page = 0;
                display_page(page);
                while (BT1 == 0);
            }
        }
    }
}