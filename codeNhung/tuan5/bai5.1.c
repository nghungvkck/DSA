#include <io.h>
#include <glcd.h>
#include <delay.h>
#include <font5x7.h>

void main(void){
    GLCDINIT_t glcd_init_data;

    // PD7 dieu khien den nen GLCD
    DDRD = 0x80;
    PORTD = 0x80;

    // Khoi tao GLCD 128x64: font 5x7, chinh tuong phan vua phai
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 80;
    glcd_init_data.bias = 3;
    glcd_init_data.vlcd = 50;
    glcd_init(&glcd_init_data);
    glcd_clear();

    // In dong chu "Hello World" gan goc tren trai
    glcd_outtextxy(10, 2, "Hello World");

    // Ve duong ke ngang duoi dong chu
    glcd_line(0, 12, 83, 12);

    // Ve hinh chu nhat nho ben trai
    glcd_rectangle(5, 20, 20, 35);

    // Ve hinh tron o giua
    glcd_circle(42, 28, 8);

    // Ve tam giac bang 3 duong thang (day + 2 canh ben)
    glcd_line(65, 35, 75, 20);
    glcd_line(75, 20, 85, 35);
    glcd_line(65, 35, 85, 35);

    while (1){}   // giu nguyen hinh ve tren man hinh
}