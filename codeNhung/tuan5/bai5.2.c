// ============================
// vi?t chuong trình hi?n th? trái tim den, kích thu?c 8x8
// khi b?m bt_1 thì trái tim nh?p nháy, khi b?m bt_2 thì trái tim t?t
// ============================

# include <io.h>
# include <glcd.h>
# include <delay.h>
# include <font5x7.h>

flash unsigned char heart[] = {  
    0x80, 0x00, 0x08, 0x00, 
    0x1C, 0x22, 0x41, 0x82, 0x82, 0x41, 0x22, 0x1C
};

flash unsigned char heart_1[] = { 
    0x80, 0x00, 0x08, 0x00, 
    0x1C, 0x3E, 0x7F, 0xFE, 0xFE, 0x7F, 0x3E, 0x1C
};

void main(void){
    GLCDINIT_t glcd_init_data; // c?u trúc d? li?u kh?i t?o GLCD
    DDRD = 0x80;               // c?u hình chân D7 là output
    PORTD = 0x80;              // c?u hình chân D7 là output
    DDRB = 0x00;               // c?u hình chân B là input
    PORTB = 0x0C;              // b?t pull-up cho chân B2 và B3

    glcd_init_data.font = font5x7;         // font s? d?ng
    glcd_init_data.temp_coef = 80;        // h? s? nhi?t d?
    glcd_init_data.bias = 3;               // h? s? bias
    glcd_init_data.vlcd = 65;              // di?n áp ngu?n
    glcd_init(&glcd_init_data);            // kh?i t?o GLCD
    glcd_putimagef(38, 20, heart, 1);          // hi?n th? trái tim den t?i t?a d? (38, 20)

    while (1){
        if (PINB.2 == 0){                  // n?u nút nh?n bt_1 du?c nh?n
            delay_ms(20);                  // ch? debounce
            if (PINB.2 == 0){
                glcd_putimagef(38, 20, heart_1, 1); // hi?n th? trái tim nh?p nháy t?i t?a d? (38, 20)
                delay_ms(500);              // ch? 500ms
                glcd_putimagef(38, 20, heart, 1);   // hi?n th? trái tim den t?i t?a d? (38, 20)
                delay_ms(500);              // ch? 500ms
            }
        }

        if (PINB.3 == 0){                  // n?u nút nh?n bt_2 du?c nh?n
            delay_ms(20);
            if (PINB.3 == 0){
                glcd_clear();              // xóa màn hình GLCD
                while (PINB.3 == 0){}          // ch? nút nh?n bt_2 du?c nh? ra
            }
        }
    }
}