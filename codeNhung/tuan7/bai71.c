// Doc ADC kenh 7, hien thi gia tri 10-bit len LCD 16x2
// Cap nhat 2 lan/giay

#include <io.h>
#include <delay.h>
#include <stdio.h>      // cho sprintf()
#include <alcd.h>

// Doc ADC 10-bit tu kenh chi dinh (0..7), tra ve 0..1023
unsigned int read_adc(unsigned char adc_input) {
    // Chon kenh + dien ap tham chieu AVCC (REFS=01), canh phai
    ADMUX = (adc_input & 0x1F) | 0x40;

    delay_us(10);                    // cho mux on dinh
    ADCSRA |= 0x40;                  // bat dau chuyen doi (ADSC=1)
    while ((ADCSRA & 0x10) == 0);    // cho co ADIF bao xong
    ADCSRA |= 0x10;                  // ghi 1 de xoa co ADIF
    return ADCW;                     // ket qua 10-bit
}

void main(void) {
    unsigned int adc;
    char buffer[17];                 // 16 ky tu + '\0'

    DDRD = 0x80;
    PORTD = 0x80;                    // bat den nen LCD

    // Khoi tao ADC: bat ADC, prescaler /128 (16MHz -> 125kHz)
    ADMUX = 0x40;
    ADCSRA = 0x87;
    SFIOR = 0x00;                    // tat auto trigger

    lcd_init(16);
    lcd_clear();

    while (1) {
        adc = read_adc(7);           // doc kenh 7

        lcd_gotoxy(0, 0);

        // In "Gia tri:" + so canh phai 4 chu so
        sprintf(buffer, "Gia tri:%4u", adc);
        lcd_puts(buffer);

        delay_ms(500);
    }
}