#include <io.h>
#include <delay.h>

void hienthi(void) {
    unsigned char i;
    for (i = 0; i <= 9; i++) {
        if (i == 0) {
            PORTF = 0xFE;   // 1111 1110
            PORTD = 0x06;   // 0000 0110
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 1) {
            PORTF = 0xF8;   // 1111 1000
            PORTD = 0x00;   // 0000 0000
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 2) {
            PORTF = 0xFA;   // 1111 1010
            PORTD = 0x0E;   // 0000 1110
            PORTA = 0x00;   // 0000 0000 
        }
        else if (i == 3) {
            PORTF = 0xFA;   // 1111 1010
            PORTD = 0x0A;   // 0000 1010
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 4) {
            PORTF = 0xFC;   // 1111 1100
            PORTD = 0x08;   // 0000 1000
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 5) {
            PORTF = 0xDE;   // 1101 1110
            PORTD = 0x0A;   // 0000 1010
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 6) {
            PORTF = 0xDE;   // 1101 1110
            PORTD = 0x0E;   // 0000 1110
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 7) {
            PORTF = 0xFA;   // 1111 1010
            PORTD = 0x00;   // 0000 0000
            PORTA = 0x80;   // 1000 0000
        }
        else if (i == 8) {
            PORTF = 0xFE;   // 1111 1110
            PORTD = 0x0E;   // 0000 1110
            PORTA = 0x80;   // 1000 0000
        }
        else {
            PORTF = 0xFE;   // 1111 1110
            PORTD = 0x0A;   // 0000 1010
            PORTA = 0x80;   // 1000 0000
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