#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>

#define BT_1 PINB.2
#define BT_2 PINB.3

#define RELAY_1 PORTC.3
#define RELAY_2 PORTC.2
#define RELAY_3 PORTC.1
#define RELAY_4 PORTC.0

eeprom unsigned int threshold_ee;

char buf[17];
int current_adc, adc_dark, adc_light, threshold;
char count = 0;
bit auto_mode = 0;

unsigned int read_adc(unsigned char adc_input) {
    ADMUX = (adc_input & 0x1F) | 0x40;
    delay_us(10);
    ADCSRA |= 0x40;
    while ((ADCSRA & 0x10) == 0);
    ADCSRA |= 0x10;
    return ADCW;
}

void role_off(void) {
    RELAY_1 = 0; 
    RELAY_2 = 0; 
    RELAY_3 = 0; 
    RELAY_4 = 0;
}

void role_on(void) {
    RELAY_1 = 1; delay_ms(1000);
    RELAY_2 = 1; delay_ms(1000);
    RELAY_3 = 1; delay_ms(1000);
    RELAY_4 = 1; delay_ms(1000);
}

void main(void) {
    DDRC = 0x0F; 
    PORTC = 0x00; 
    DDRB = 0x00;
    PORTB = 0x0C; 
    DDRD = 0x80; 
    PORTD = 0x80;
    ADMUX = 0x40; 
    ADCSRA = 0x87; 
    SFIOR = 0x00;

    lcd_init(16); 
    lcd_clear();

    threshold = threshold_ee;
    lcd_gotoxy(0, 0); 
    lcd_puts("BT-1: Nguong");
    lcd_gotoxy(0, 1); 
    lcd_puts("BT-2: Auto");

    while (1) {
        if (BT_1 == 0) {
            delay_ms(20);
            if (BT_1 == 0) {
                count++;
                if (count == 1) {
                    adc_dark = read_adc(7);
                    lcd_clear();
                    lcd_gotoxy(0, 0); 
                    lcd_puts("L1: ADC TOI");
                    sprintf(buf, "%4u", adc_dark);
                    lcd_gotoxy(0, 1); 
                    lcd_puts(buf);
                }
                else if (count == 2) {
                    adc_light = read_adc(7);
                    lcd_clear();
                    lcd_gotoxy(0, 0); 
                    lcd_puts("L2: ADC SANG");
                    sprintf(buf, "%4u", adc_light);
                    lcd_gotoxy(0, 1); 
                    lcd_puts(buf);
                }
                else if (count == 3) {
                    threshold = (adc_dark + adc_light) / 2;
                    threshold_ee = threshold;
                    count = 0;
                    lcd_clear();
                    lcd_gotoxy(0, 0); 
                    lcd_puts("L3: NGUONG");
                    sprintf(buf, "%4u", threshold);
                    lcd_gotoxy(0, 1); 
                    lcd_puts(buf);
                }
                while (BT_1 == 0);
            }
        }

        if (BT_2 == 0) {
            delay_ms(20);
            if (BT_2 == 0) {
                auto_mode = !auto_mode;
                if (!auto_mode) role_off();
                lcd_clear(); 
                lcd_gotoxy(0, 0);
                lcd_puts(auto_mode ? "AUTO: ON" : "AUTO: OFF");
                while (BT_2 == 0);
            }
        }

        if (auto_mode) {
            current_adc = read_adc(7);
            if (current_adc < threshold) role_on();
            else role_off();

            lcd_gotoxy(0, 0);
            sprintf(buf, "ADC:%4u", current_adc);
            lcd_puts(buf);
            delay_ms(100);
        }
    }
}