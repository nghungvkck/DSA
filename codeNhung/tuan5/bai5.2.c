// ============================
// viết chương trình hiển thị trái tim đen, kích thước 8x8
// khi bấm bt_1 thì trái tim nhấp nháy, khi bấm bt_2 thì trái tim tắt
// ============================

# include <io.h>
# include <glcd.h>
# include <delay.h>
# include <font5x7.h>

flash unsigned char heart[] = {   // định nghĩa mảng dữ liệu bitmap trong bộ nhớ chương trình flash
    0x80, 0x00, 0x08, 0x00, 
    0x1C, 0x22, 0x41, 0x82, 0x82, 0x41, 0x22, 0x1C
};

flash unsigned char heart_1[] = { // định nghĩa mảng dữ liệu bitmap trong bộ nhớ chương trình flash
    0x80, 0x00, 0x08, 0x00, 
    0x1C, 0x3E, 0x7F, 0xFE, 0xFE, 0x7F, 0x3E, 0x1C
};

void main(void){
    GLCDINIT_t glcd_init_data; // cấu trúc dữ liệu khởi tạo GLCD
    DDRD = 0x80;               // cấu hình chân D7 là output
    PORTD = 0x80;              // cấu hình chân D7 là output
    DDRB = 0x00;               // cấu hình chân B là input
    PORTB = 0x0C;              // bật pull-up cho chân B2 và B3

    glcd_init_data.font = font5x7;         // font sử dụng
    glcd_init_data.temp _coef = 80;        // hệ số nhiệt độ
    glcd_init_data.bias = 3;               // hệ số bias
    glcd_init_data.vlcd = 65;              // điện áp nguồn
    glcd_init(&glcd_init_data);            // khởi tạo GLCD
    glcd_putimage(38, 20, heart, 1);          // hiển thị trái tim đen tại tọa độ (38, 20)

    while (1){
        if (PINB.2 == 0){                  // nếu nút nhấn bt_1 được nhấn
            delay_ms(20);                  // chờ debounce
            if (PINB.2 == 0){
                glcd_putimage(38, 20, heart_1, 1); // hiển thị trái tim nhấp nháy tại tọa độ (38, 20)
                delay_ms(500);              // chờ 500ms
                glcd_putimage(38, 20, heart, 1);   // hiển thị trái tim đen tại tọa độ (38, 20)
                delay_ms(500);              // chờ 500ms
            }
        }

        if (PINB.3 == 0){                  // nếu nút nhấn bt_2 được nhấn
            delay_ms(20);
            if (PINB.3 == 0){
                glcd_clear();              // xóa màn hình GLCD
                while (PINB.3 == 0){}          // chờ nút nhấn bt_2 được nhả ra
            }
        }
    }
}