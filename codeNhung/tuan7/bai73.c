// Bắt đầu:
//     Khởi tạo GLCD
//     Khởi tạo ADC
//     Khởi tạo I/O

//     Vòng lặp vô hạn:
//         // 1. Đọc dữ liệu
//         GiaTriADC = Đọc_ADC()
//         PhanTram = (GiaTriADC / 1023) * 100

//         // 2. Điều khiển LED (Ngưỡng 30%)
//         Nếu (PhanTram < 30):
//             Bật LED
//         Ngược lại:
//             Tắt LED

//         // 3. Hiển thị trạng thái (Ngưỡng 40%)
//         Nếu (PhanTram >= 40):
//             Hiển thị "Trời sáng"
//         Ngược lại:
//             Hiển thị "Trời tối"

//         // 4. Lặp lại
//         Quay lại Vòng lặp vô hạn
// Kết thúc

#include <mega128.h>
#include <delay.h>
#include <stdio.h>
#include <glcd.h>
#include <font5x7.h>

// ========================================
// 1. BITMAP ICON MAT TROI
// ========================================

flash unsigned char sun_icon[] = {
    0x20, 0x00, 0x1E, 0x00,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
    0x00, 0x10, 0x20, 0x80, 0x80, 0xC0, 0xC0, 0xDE,
    0xC0, 0xC0, 0x80, 0x80, 0x20, 0x10, 0x00, 0x00,
    0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01,
    0x00, 0x40, 0x40, 0x40, 0x40, 0x42, 0x04, 0xE0,
    0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC,
    0xFF, 0xFF, 0xFE, 0xFF, 0xFE, 0xFF, 0xFC, 0xF0,
    0x00, 0x00, 0x84, 0x82, 0x80, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x08, 0x43,
    0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x3F, 0x3F, 0x1F, 0x0F, 0x07, 0x41,
    0x80, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00,
    0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1F,
    0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x00
};

// Icon mat troi mo - du lieu goc 44 byte
flash unsigned char sun_dim_icon[] = {
    0x20, 0x00, 0x1E, 0x00,
    0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
    0x00, 0x10, 0x20, 0x80, 0x80, 0x40, 0x40, 0x5E,
    0x40, 0x40, 0x80, 0x80, 0x20, 0x10, 0x08, 0x00,
    0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00,
    0x40, 0x40, 0x40, 0x40, 0x42, 0x04, 0xF0, 0x0C
};

// ========================================
// 2. KHAI BAO BIEN
// ========================================

#define LED PORTD.4

char buffer[20];

// ========================================
// 3. HAM DOC ADC
// ========================================

unsigned int read_adc(unsigned char adc_input)
{
    // Chon dien ap tham chieu AVCC va kenh ADC
    ADMUX = (adc_input & 0x07) | 0x40;

    // Bat dau chuyen doi ADC
    ADCSRA |= (1 << ADSC);

    // Cho chuyen doi hoan tat
    while (ADCSRA & (1 << ADSC));

    // Tra ve ket qua ADC 10-bit
    return ADCW;
}

// ========================================
// 4. CHUONG TRINH CHINH
// ========================================

void main(void)
{
    unsigned int adc;
    unsigned int percent;

    GLCDINIT_t glcd_init_data;

    // PD4 la OUTPUT, PD7 la INPUT
    DDRD = 0x10;

    // Bat dien tro keo len cho PD7
    PORTD = 0x80;

    // Cau hinh ADC: AVCC, prescaler 128
    ADMUX = 0x40;
    ADCSRA = 0x87;
    SFIOR = 0x00;

    // Khoi tao GLCD
    glcd_init_data.font = font5x7;
    glcd_init(&glcd_init_data);

    // Man hinh khoi dong
    glcd_clear();
    glcd_outtextxy(0, 2, "LIGHT SENSOR");
    glcd_outtextxy(0, 15, "SYSTEM START");
    delay_ms(1500);
    glcd_clear();

    // Vong lap chinh
    while (1)
    {
        // Doc gia tri cam bien tai ADC7
        adc = read_adc(7);

        // Chuyen ADC thanh phan tram 0..100
        percent = (unsigned int)(((unsigned long)adc * 100) / 1023);

        // Dieu khien LED theo nguong 60%
        if (percent >= 60)
        {
            LED = 1;
        }
        else
        {
            LED = 0;
        }

        // Hien thi icon theo nguong 70%
        if (percent >= 70)
        {
            glcd_putimagef(0, 0, sun_icon, 1);
        }
        else
        {
            glcd_putimagef(0, 0, sun_dim_icon, 1);
        }

        // Ve duong phan cach
        glcd_line(50, 0, 50, 48);

        // Hien thi phan tram anh sang
        sprintf(buffer, "%u%%", percent);
        glcd_outtextxy(55, 20, buffer);

        // Hien thi gia tri ADC
        sprintf(buffer, "ADC:%u", adc);
        glcd_outtextxy(55, 35, buffer);

        delay_ms(200);

        // Xoa vung hien thi truoc khi cap nhat
        // Luu y: neu xoa toan bo man hinh, icon se nhap nhay.
        // Vi vay nen chi xoa vung chu neu thu vien ho tro.
    }
}