#include <io.h>
#include <alcd.h>
#include <delay.h>

// In noi dung co dinh len LCD: dong 0 "He lo", dong 1 "Bai 4"
void show_text(void) {
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_puts("He lo");
    lcd_gotoxy(0, 1);
    lcd_puts("Bai 4");
}

void main(void) {
    unsigned char i;

    DDRD  = 0x80;   // PD7 ngo ra dieu khien den nen
    PORTD = 0x80;   // Bat den nen LCD

    lcd_init(16);
    lcd_clear();

    // Nhap nhay 2 lan: hien 1s, xoa 1s
    for (i = 0; i < 2; i++) {
        show_text();
        delay_ms(1000);
        lcd_clear();
        delay_ms(1000);
    }

    // Hien lai lan cuoi va giu nguyen
    show_text();

    while (1) {}
}