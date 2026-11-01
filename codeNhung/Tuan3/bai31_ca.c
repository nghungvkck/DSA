#include <io.h>
#include <delay.h>

void hienthi(void) {
    unsigned char i;
    for (i = 0; i <= 9; i++) {
        if (i == 0) {
            PORTF = ~0xFE;   // Dao cua 1111 1110 la 0000 0001
            PORTD = ~0x06;   // Dao cua 0000 0110 la 1111 1001 (Ma so 0 chuan CA)
            PORTA = ~0x80;   // Dao cua 1000 0000 la 0111 1111
        }
        else if (i == 1) {
            PORTF = ~0xF8;
            PORTD = ~0x00;   // Dao cua 0000 0000 la 1111 1111 (Ma so 1 chuan CA)
            PORTA = ~0x80;
        }
        else if (i == 2) {
            PORTF = ~0xFA;
            PORTD = ~0x0E;   // Dao thanh 1111 0001 (Ma so 2)
            PORTA = ~0x00;   // Dao thanh 1111 1111
        }
        else if (i == 3) {
            PORTF = ~0xFA;
            PORTD = ~0x0A;   // Dao thanh 1111 0101 (Ma so 3)
            PORTA = ~0x80;
        }
        else if (i == 4) {
            PORTF = ~0xFC;
            PORTD = ~0x08;   // Dao thanh 1111 0111 (Ma so 4)
            PORTA = ~0x80;
        }
        else if (i == 5) {
            PORTF = ~0xDE;
            PORTD = ~0x0A;   // Dao thanh 1111 0101 (Ma so 5)
            PORTA = ~0x80;
        }
        else if (i == 6) {
            PORTF = ~0xDE;
            PORTD = ~0x0E;   // Dao thanh 1111 0001 (Ma so 6)
            PORTA = ~0x80;
        }
        else if (i == 7) {
            PORTF = ~0xFA;
            PORTD = ~0x00;   // Dao thanh 1111 1111 (Ma so 7)
            PORTA = ~0x80;
        }
        else if (i == 8) {
            PORTF = ~0xFE;
            PORTD = ~0x0E;   // Dao thanh 1111 0001 (Ma so 8)
            PORTA = ~0x80;
        }
        else {
            PORTF = ~0xFE;
            PORTD = ~0x0A;   // Dao thanh 1111 0101 (Ma so 9)
            PORTA = ~0x80;
        }
        delay_ms(1000);
    }
}

void main(void) {
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;

    while (1) {
        hienthi();
    }
}