#include <io.h>
#include <delay.h>

int mode = 0;

void hienthi(int i, unsigned char select_mask)
{
    unsigned char f_data;
    PORTA = 0x00;
    PORTD = 0x00;
    f_data = 0x00;

    if (i == 1) {
        f_data = 0x20
        PORTA = 0x80;
        PORTD = 0x00;
    }

    if (i == 2) {
        f_data = 0x22 | select_mask;
        PORTA = 0x00;
        PORTD = 0x0E;
    }

    if (i == 3) {
        f_data = 0x22 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0A;
    }
    PORTF = f_data | select_mask;
}

void updateDisplay(){
    if (mode == 1) hienthi(1, 0x58);
    else if (mode == 2) hienthi (2, 0x51);
    else if ( mode == 3) hienthi(3, 0x49 );
    else {
        PORTF = 0x59;
        PORTA = 0x00;
        PORTD = 0x00;
    }
}

void main(){
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;
    DDRB = 0x00;
    PORTB = 0x0D;
    while (1)
    {
        if ( PINB.2 == 0){
            delay_ms(20);
        }
    }
    
}