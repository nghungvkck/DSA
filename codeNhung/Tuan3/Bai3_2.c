#include <io.h>
#include <delay.h>
#include <stdio.h>

int count = 0;

void hienthi(int i, unsigned char select_mask)
{    
    // dùng vòng lặp for duỵet
    for 
    PORTF &= 0x00;
    PORTA &= 0x7F;
    PORTD &= 0xF1;

    if (i == 0) {   
        PORTF = 0x26 | select_mask;
        PORTA = 0x80;
        PORTD = 0x06;
    }

    if (i == 1) {
        PORTF = 0x20 | select_mask;
        PORTA = 0x80;
        PORTD = 0x00;
    }

    if (i == 2) {
        PORTF = 0x22 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0E;
    }

    if (i == 3) {
        PORTF = 0x22 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0A;
    }

    if (i == 4) {
        PORTF = 0x24 | select_mask;
        PORTA = 0x80;
        PORTD = 0x08;
    }

    if (i == 5) {
        PORTF = 0x06 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0A;
    }

    if (i == 6) {
        PORTF = 0x06 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0E;
    }

    if (i == 7) {
        PORTF = 0x22 | select_mask;
        PORTA = 0x80;
        PORTD = 0x00;
    }

    if (i == 8) {
        PORTF = 0x26 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0E;
    }

    if (i == 9) {
        PORTF = 0x26 | select_mask;
        PORTA = 0x80;
        PORTD = 0x0A;
    }
}

void refreshDisplay()
{
    hienthi(count / 10, 0x58);
    delay_ms(5);

    hienthi(count % 10, 0x51);
    delay_ms(5);
}

void main()
{
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;

    DDRB = 0x00;
    PORTB = 0x0D;

    while (1)
    {
        if (PINB.2 == 0)
        {
            delay_ms(20);

            if (PINB.2 == 0)
            {
                if (count < 99)
                    count++;

                while (PINB.2 == 0)
                {
                    refreshDisplay();
                }
            }
        }

        if (PINB.3 == 0)
        {
            delay_ms(20);

            if (PINB.3 == 0)
            {
                if (count > 0)
                    count--;

                while (PINB.3 == 0)
                {
                    refreshDisplay();
                }
            }
        }

        if (PINB.0 == 0)
        {
            delay_ms(20);
            if (PINB.0 == 0)
            {
                count = 0;

                while (PINB.0 == 0)
                {
                    refreshDisplay();
                }
            }
        }

        refreshDisplay();
    }
}