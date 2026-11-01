// Hien thi trai tim den 8x8 tren GLCD
// BT_1 (PB2): trai tim nhap nhay (den <-> day)
// BT_2 (PB3): tat (xoa) trai tim

#include <io.h>
#include <glcd.h>
#include <delay.h>
#include <font5x7.h>

// Trai tim rong (vien den, ruot trang) - 8x8
flash unsigned char heart[] = {
    0x80, 0x00, 0x08, 0x00,           // header: width=8, height=8
    0x1C, 0x22, 0x41, 0x82, 0x82, 0x41, 0x22, 0x1C
};

// Trai tim day (to dac) - 8x8
flash unsigned char heart_1[] = {
    0x80, 0x00, 0x08, 0x00,           // header: width=8, height=8
    0x1C, 0x3E, 0x7F, 0xFE, 0xFE, 0x7F, 0x3E, 0x1C
};

void main(void){
    GLCDINIT_t glcd_init_data;

    DDRD = 0x80;
    PORTD = 0x80;                 // bat den nen GLCD
    DDRB = 0x00;
    PORTB = 0x0C;                 // pull-up cho PB2, PB3

    // Khoi tao GLCD 128x64
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 80;
    glcd_init_data.bias = 3;
    glcd_init_data.vlcd = 55;
    glcd_init(&glcd_init_data);

    glcd_putimagef(38, 20, heart, 1);   // ve tim rong o giua

    while (1){
        if (PINB.2 == 0){                 // BT_1: nhap nhay tim
            delay_ms(20);                 // chong doi
            if (PINB.2 == 0){
                glcd_putimagef(38, 20, heart_1, 1);   // tim day
                delay_ms(500);
                glcd_putimagef(38, 20, heart, 1);     // tim rong
                delay_ms(500);
            }
        }

        if (PINB.3 == 0){                 // BT_2: xoa tim
            delay_ms(20);
            if (PINB.3 == 0){
                glcd_clear();
                while (PINB.3 == 0){}     // cho nha nut
            }
        }
    }
}