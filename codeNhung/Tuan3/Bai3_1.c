#include <io.h>
#include <delay.h>

void hienthi(void){
    unsigned char i = 0;
    for ( i= 0; i<= 9; i++){
        if ( i ==0){
            PORTF =  0xFE;
            PORTD = 0X66;
            PORTA = 0X80;
        }
    } else if (i == 1)
    {
        PORTF = 0XF8;
        PORTD = 0X00;
        PORTA = 0X80;
    }
}

void main(void){
    DDRF = 0XFF;
    DDRD = 0XFF;
    DDRA = 0XFF;
    while (1)
    {
        hienthi();
    }
    
}