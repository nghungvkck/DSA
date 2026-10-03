// ==========================
//  viết chương trình hiển thị dong chữ hello world
// ==========================

# include <io.h>
# include <alcd.h>
# include <delay.h>
# include <font5x7.h>

void main(void){
    GLCDINIT_t glcd_init_data; // cấu trúc dữ liệu khởi tạo GLCD
    DDRD = 0x80;               // cấu hình chân D7 là output
    PORTD = 0x80;              // cấu hình chân D7 là output

    glcd_init_data.font = font5x7;         // font sử dụng
    glcd_init_data.temp _coef = 80;        // hệ số nhiệt độ
    glcd_init_data.bias = 3;               // hệ số bias
    glcd_init_data.vlcd = 50;              // điện áp nguồn
    glcd_init(&glcd_init_data);            // khởi tạo GLCD
    glcd_clear();                          // xóa màn hình GLCD

    glcd_outtextxy(10, 10, "Hello World"); // hiển thị chữ Hello World tại tọa độ (10, 10)
    glcd_line(0, 12, 83, 12);              // vẽ đường thẳng từ (0, 12) đến (83, 12)
    glcd_rectangle(5, 20, 20, 35);         // vẽ hình chữ nhật từ (5, 20) đến (20, 35)
    glcd_circle(42, 28, 8);                // vẽ hình tròn tâm (42, 28) bán kính 8
    glcd_line(65, 35, 75, 20);             // vẽ đường thẳng từ (65, 35) đến (75, 20)
    glcd_line(75, 20, 85, 35);             // vẽ đường thẳng từ (75, 20) đến (85, 35)
    glcd_line(65, 35, 85, 35);             // vẽ đường thẳng từ (65, 35) đến (85, 35)
    while (1){}                            // vòng lặp vô hạn để giữ chương trình chạy
}