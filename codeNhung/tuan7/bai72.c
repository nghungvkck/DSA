// Bắt đầu:
//     Khởi tạo LCD
//     Khởi tạo ADC
//     Khởi tạo I/O
//     dem = 0

//     Vòng lặp vô hạn:
//         // 1. Đọc dữ liệu
//         Ngưỡng = Đọc_EEPROM()
//         GiaTriADC = Đọc_ADC()

//         // 2. Điều khiển Relay
//         Nếu (GiaTriADC < Ngưỡng):
//             Tắt Role
//         Ngược lại:
//             Bật Role

//         // 3. Kiểm tra nút nhấn BT_2
//         Nếu (BT_2 == Được_nhấn):
//             dem = dem + 1

//             Nếu (dem == 1):
//                 ADC_toi = Đọc_ADC()
//             Ngược lại nếu (dem == 2):
//                 ADC_sang = Đọc_ADC()
//             Ngược lại nếu (dem == 3):
//                 Ngưỡng_moi = (ADC_toi + ADC_sang) / 2
//                 Ghi_EEPROM(Ngưỡng_moi)
//                 dem = 0

//         // 4. Lặp lại
//         Chờ một khoảng thời gian (delay)
//         Quay lại Vòng lặp vô hạn
// Kết thúc


#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>

// =====================================
// PIN DEFINITIONS
// =====================================

// Button BT2: PD2, active LOW
#define BT_2 PIND.2

// Relays: PC0-PC3, active LOW
#define RELAY_1 PORTC.0
#define RELAY_2 PORTC.1
#define RELAY_3 PORTC.2
#define RELAY_4 PORTC.3

// =====================================
// EEPROM
// =====================================
eeprom unsigned int threshold_ee;

// =====================================
// GLOBAL VARIABLES
// =====================================
unsigned char dem = 0;

unsigned int adc_dark = 0;
unsigned int adc_light = 0;
unsigned int threshold = 0;
unsigned int current_adc = 0;

char buf[17];

// =====================================
// READ ADC
// =====================================
unsigned int read_adc(unsigned char adc_input)
{
    ADMUX = (adc_input & 0x07) | 0x40;

    delay_us(10);

    // Start conversion
    ADCSRA |= (1 << ADSC);

    // Wait until conversion completes
    while (ADCSRA & (1 << ADSC));

    return ADCW;
}

// =====================================
// RELAY CONTROL (ACTIVE LOW)
// =====================================
void role_on(void)
{
    RELAY_1 = 0;
    RELAY_2 = 0;
    RELAY_3 = 0;
    RELAY_4 = 0;
}

void role_off(void)
{
    RELAY_1 = 1;
    RELAY_2 = 1;
    RELAY_3 = 1;
    RELAY_4 = 1;
}

// =====================================
// DISPLAY LCD LINE
// =====================================
void show_line(unsigned char row, char *s)
{
    unsigned char i = 0;

    lcd_gotoxy(0, row);

    while (i < 16 && s[i] != '\0')
    {
        lcd_putchar(s[i]);
        i++;
    }

    while (i < 16)
    {
        lcd_putchar(' ');
        i++;
    }
}

// =====================================
// MAIN PROGRAM
// =====================================
void main(void)
{
    // 1. Initialize I/O
    DDRC = (DDRC & 0xF0) | 0x0F;

    // All relays OFF initially
    role_off();

    // BT2 input with internal pull-up
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);

    // 2. Initialize ADC
    ADMUX = 0x40;       // AVCC reference
    ADCSRA = 0x87;      // ADC enable, prescaler 128
    SFIOR = 0x00;

    // 3. Initialize LCD
    lcd_init(16);
    lcd_clear();

    dem = 0;

    show_line(0, "SYSTEM READY");
    show_line(1, "INITIALIZING...");
    delay_ms(1000);

    // =================================
    // INFINITE LOOP
    // =================================
    while (1)
    {
        // ---------------------------------
        // 1. READ EEPROM AND ADC
        // ---------------------------------
        threshold = threshold_ee;
        current_adc = read_adc(7);

        // ---------------------------------
        // 2. CONTROL RELAYS
        // ---------------------------------
        if (current_adc < threshold)
        {
            role_off();
        }
        else
        {
            role_on();
        }

        // ---------------------------------
        // 3. CHECK BUTTON BT2
        // ---------------------------------
        if (BT_2 == 0)
        {
            delay_ms(20);

            if (BT_2 == 0)
            {
                dem++;

                if (dem == 1)
                {
                    // First press: measure darkness
                    adc_dark = read_adc(7);

                    show_line(0, "CALIBRATION 1");
                    sprintf(buf, "DARK: %4u", adc_dark);
                    show_line(1, buf);
                }
                else if (dem == 2)
                {
                    // Second press: measure bright light
                    adc_light = read_adc(7);

                    show_line(0, "CALIBRATION 2");
                    sprintf(buf, "LIGHT: %4u", adc_light);
                    show_line(1, buf);
                }
                else if (dem == 3)
                {
                    // Third press: calculate and save
                    threshold = (adc_dark + adc_light) / 2;

                    // Save new threshold to EEPROM
                    threshold_ee = threshold;

                    dem = 0;

                    show_line(0, "THRESHOLD SAVED");
                    sprintf(buf, "TH: %4u", threshold);
                    show_line(1, buf);

                    delay_ms(1000);
                }

                // Wait for button release
                while (BT_2 == 0);

                delay_ms(20);
            }
        }

        // ---------------------------------
        // 4. UPDATE LCD WHEN NOT CALIBRATING
        // ---------------------------------
        if (dem == 0)
        {
            sprintf(buf, "ADC: %4u", current_adc);
            show_line(0, buf);

            sprintf(buf, "TH: %4u", threshold);
            show_line(1, buf);
        }

        // ---------------------------------
        // 5. DELAY
        // ---------------------------------
        delay_ms(100);
    }
}
