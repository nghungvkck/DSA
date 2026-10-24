#include <io.h>
#include <delay.h>

void hienthi(void) {
    unsigned char i;
    for (i = 0; i <= 9; i++) {
        if (i == 0) {
            PORTF = ~0xFE;   // Ð?o c?a 1111 1110 là 0000 0001
            PORTD = ~0x06;   // Ð?o c?a 0000 0110 là 1111 1001 (Mã s? 0 chu?n CA)
            PORTA = ~0x80;   // Ð?o c?a 1000 0000 là 0111 1111
        }
        else if (i == 1) {
            PORTF = ~0xF8;   
            PORTD = ~0x00;   // Ð?o c?a 0000 0000 là 1111 1111 (Mã s? 1 chu?n CA)
            PORTA = ~0x80;   
        }
        else if (i == 2) {
            PORTF = ~0xFA;   
            PORTD = ~0x0E;   // Ð?o thành 1111 0001 (Mã s? 2)
            PORTA = ~0x00;   // Ð?o thành 1111 1111
        }
        else if (i == 3) {
            PORTF = ~0xFA;   
            PORTD = ~0x0A;   // Ð?o thành 1111 0101 (Mã s? 3)
            PORTA = ~0x80;   
        }
        else if (i == 4) {
            PORTF = ~0xFC;   
            PORTD = ~0x08;   // Ð?o thành 1111 0111 (Mã s? 4)
            PORTA = ~0x80;   
        }
        else if (i == 5) {
            PORTF = ~0xDE;   
            PORTD = ~0x0A;   // Ð?o thành 1111 0101 (Mã s? 5)
            PORTA = ~0x80;   
        }
        else if (i == 6) {
            PORTF = ~0xDE;   
            PORTD = ~0x0E;   // Ð?o thành 1111 0001 (Mã s? 6)
            PORTA = ~0x80;   
        }
        else if (i == 7) {
            PORTF = ~0xFA;   
            PORTD = ~0x00;   // Ð?o thành 1111 1111 (Mã s? 7)
            PORTA = ~0x80;   
        }
        else if (i == 8) {
            PORTF = ~0xFE;   
            PORTD = ~0x0E;   // Ð?o thành 1111 0001 (Mã s? 8)
            PORTA = ~0x80;   
        }
        else {
            PORTF = ~0xFE;   
            PORTD = ~0x0A;   // Ð?o thành 1111 0101 (Mã s? 9)
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