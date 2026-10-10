#include <io.h>
#include <delay.h>
#include <i2c.h>
#include <math.h>
#include <stdio.h>
#include <glcd.h>
#include <font5x7.h>
#include <stdlib.h>
#define MPU6050_ADDR 0x68
int read_raw(unsigned char reg) {
    unsigned char h, l;
    int value = 0;
    i2c_start();
    if (i2c_write(MPU6050_ADDR << 1)) {
        i2c_write(reg);
        i2c_start();
        i2c_write((MPU6050_ADDR << 1) | 1);
        h = i2c_read(1);
        l = i2c_read(0);
        value = (int)((h << 8) | l);
    }
    i2c_stop();
    return value;
}
void main(void) {
    GLCDINIT_t glcd_init_data;
    int ax, ay, az;
    float f_angleX, f_angleY, f_angleZ;
    int ix, iy, iz;
    char buf[16];
    DDRF = 0xFF; PORTF = 0x00;
    DDRA = 0x80; PORTA &= ~0x80;
    DDRD = 0x0E; PORTD &= ~0x0E;
    DDRD.7 = 1; PORTD.7 = 1;
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 63;
    glcd_init_data.bias = 4;
    glcd_init_data.vlcd = 50;
    glcd_init(&glcd_init_data);
    i2c_init();
    i2c_start();
    i2c_write(MPU6050_ADDR << 1);
    i2c_write(0x6B); i2c_write(0x00);
    i2c_stop();
    delay_ms(100);
    glcd_clear();
    while (1) {
        ax = read_raw(0x3B);
        ay = read_raw(0x3D);
        az = read_raw(0x3F);
        if (az == 0) az = 1;
        f_angleX = atan2((float)ay, (float)az) * 57.2957;
        f_angleY = atan2((float)ax, (float)az) * 57.2957;
        f_angleZ = acos((float)az / 16384.0) * 57.2957;
        ix = (int)(f_angleX * 10);
        iy = (int)(f_angleY * 10);
        iz = (int)(f_angleZ * 10);
        glcd_outtextxy(0, 0, "MPU6050 Angles");
        sprintf(buf,"X:%4d.%d deg",ix/10,abs(ix % 10));
        glcd_outtextxy(0, 12, buf);
        sprintf(buf,"Y:%4d.%d deg",iy/10,abs(iy % 10));
        glcd_outtextxy(0, 22, buf);
        sprintf(buf,"Z:%4d.%d deg",iz/10,abs(iz % 10));
        glcd_outtextxy(0, 32, buf);
        if (abs(ix)<30 && abs(iy)<30)
            glcd_outtextxy(20, 42, "[ OK ]");
        else glcd_outtextxy(10, 42, "[ TILTED ]");
        delay_ms(150); }
}