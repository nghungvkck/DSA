// Dieu khien 4 relay theo anh sang (ADC) - ATmega128
// - BT_1 (PB2): hieu chinh nguong 3 buoc (toi -> sang -> tinh TB)
// - BT_2 (PB3): bat/tat che do tu dong
// - AUTO ON: ADC < nguong (toi) -> bat relay tuan tu
//            ADC >= nguong (sang) -> tat het relay

#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>

#define BT_1 PINB.2
#define BT_2 PINB.3

// 4 relay xuat ra PORTC
#define RELAY_1 PORTC.3
#define RELAY_2 PORTC.2
#define RELAY_3 PORTC.1
#define RELAY_4 PORTC.0

// Nguong luu EEPROM de khong mat khi mat dien
eeprom unsigned int threshold_ee;

char buf[17];
int current_adc, adc_dark, adc_light, threshold;
char count = 0;              // dem so lan nhan BT_1 (1..3)
bit auto_mode = 0;           // 0 = tat auto, 1 = bat auto

// Doc ADC 10-bit tu kenh chi dinh
unsigned int read_adc(unsigned char adc_input) {
    ADMUX = (adc_input & 0x1F) | 0x40;   // REFS=01 (AVCC), chon kenh
    delay_us(10);
    ADCSRA |= 0x40;                      // bat dau chuyen doi
    while ((ADCSRA & 0x10) == 0);        // cho ADIF
    ADCSRA |= 0x10;                      // xoa co
    return ADCW;
}

void role_off(void) {
    RELAY_1 = 0; RELAY_2 = 0; RELAY_3 = 0; RELAY_4 = 0;
}

// Bat tuan tu 1->2->3->4, moi cai cach 1s de tranh sut ap nguon
void role_on(void) {
    RELAY_1 = 1; delay_ms(1000);
    RELAY_2 = 1; delay_ms(1000);
    RELAY_3 = 1; delay_ms(1000);
    RELAY_4 = 1; delay_ms(1000);
}

void main(void) {
    // Cau hinh chan
    DDRC  = 0x0F;              // PC0..PC3 = output relay
    PORTC = 0x00;              // ban dau tat het
    DDRB  = 0x00;              // PORTB input doc nut
    PORTB = 0x0C;              // pull-up cho PB2, PB3
    DDRD  = 0x80;
    PORTD = 0x80;              // bat den nen LCD

    // Khoi tao ADC: bat, prescaler /128
    ADMUX  = 0x40;
    ADCSRA = 0x87;
    SFIOR  = 0x00;

    lcd_init(16);
    lcd_clear();

    threshold = threshold_ee;  // nap nguong da luu

    lcd_gotoxy(0, 0); lcd_puts("BT-1: Nguong");
    lcd_gotoxy(0, 1); lcd_puts("BT-2: Auto");

    while (1) {
        // BT_1: hieu chinh nguong 3 buoc
        if (BT_1 == 0) {
            delay_ms(20);
            if (BT_1 == 0) {
                count++;

                if (count == 1) {
                    // Buoc 1: lay mau khi TOI
                    adc_dark = read_adc(7);
                    lcd_clear();
                    lcd_gotoxy(0, 0); lcd_puts("L1: ADC TOI");
                    sprintf(buf, "%4u", adc_dark);
                    lcd_gotoxy(0, 1); lcd_puts(buf);
                }
                else if (count == 2) {
                    // Buoc 2: lay mau khi SANG
                    adc_light = read_adc(7);
                    lcd_clear();
                    lcd_gotoxy(0, 0); lcd_puts("L2: ADC SANG");
                    sprintf(buf, "%4u", adc_light);
                    lcd_gotoxy(0, 1); lcd_puts(buf);
                }
                else if (count == 3) {
                    // Buoc 3: tinh trung binh, ghi EEPROM
                    threshold = (adc_dark + adc_light) / 2;
                    threshold_ee = threshold;
                    count = 0;
                    lcd_clear();
                    lcd_gotoxy(0, 0); lcd_puts("L3: NGUONG");
                    sprintf(buf, "%4u", threshold);
                    lcd_gotoxy(0, 1); lcd_puts(buf);
                }
                while (BT_1 == 0);      // cho nha nut
            }
        }

        // BT_2: dao che do AUTO
        if (BT_2 == 0) {
            delay_ms(20);
            if (BT_2 == 0) {
                auto_mode = !auto_mode;
                if (!auto_mode) role_off();   // vua tat -> tat het relay
                lcd_clear();
                lcd_gotoxy(0, 0);
                lcd_puts(auto_mode ? "AUTO: ON" : "AUTO: OFF");
                while (BT_2 == 0);
            }
        }

        // AUTO: tu dieu khien relay theo anh sang
        if (auto_mode) {
            current_adc = read_adc(7);

            // Toi (ADC nho hon nguong) -> bat relay; sang -> tat
            if (current_adc < threshold) role_on();
            else role_off();

            lcd_gotoxy(0, 0);
            sprintf(buf, "ADC:%4u", current_adc);
            lcd_puts(buf);
            delay_ms(100);
        }
    }
}