#include <io.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>
unsigned int read_adc(unsigned char adc_input) {
    ADMUX = (adc_input & 0x1F) | 0x40;
    delay_us(10); ADCSRA |= 0x40;
    while ((ADCSRA & 0x10) == 0);
    ADCSRA |= 0x10;
    return ADCW; }
void main(void) {
    unsigned int adc;
    char buffer[17];
    DDRD = 0x80; PORTD = 0x80;
    ADMUX = 0x40; ADCSRA = 0x87; SFIOR = 0x00;
    lcd_init(16); lcd_clear();
    while (1) {
        adc = read_adc(7); lcd_gotoxy(0, 0);
        sprintf(buffer, "Gia tri:%4u", adc);
        lcd_puts(buffer); delay_ms(500); }
}