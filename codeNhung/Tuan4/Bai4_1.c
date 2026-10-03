#include <io.h>
#include <alcd.h>
#include <delay.h>
 
void show_text(void){
    lcd_clear();
    lcd_gotoxy(0,0);
    lcd_puts("Hello");
    lcd_gotoxy(0,1);
    lcd_puts("Bai 4");
}

void main(void){
    unsigned char i;
    DDRD = 0x80 ;
    PORTD = 0X80;
    lcd_init(16);
    lcd_clear();
    for ( int i =0; i< 2; i++){
        show_text();
        delay_ms(1000);
        lcd_clear();
        delay_ms(1000);  
    }
    show_text();
    while (1){}
}