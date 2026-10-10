#include <io.h>
#include <mega128.h>
#include <delay.h>
#include <font5x7.h>
#include <glcd.h>
#include <stdio.h>
#include <stdlib.h>

#define CE PORTE.3
#define CSN PORTE.4
#define SCK PORTE.5
#define MOSI PORTE.6
#define MISO PINE.2
#define IRQ PINB.2
#define BT3 PINB.0

unsigned char P_Add = 0xB5;
unsigned char mode = 0, x_pos = 0, y_pos = 10;

unsigned char SPI_RW(unsigned char Buff) {
    unsigned char bit_ctr;
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        MOSI = (Buff & 0x80);
        delay_us(5);
        Buff = (Buff << 1);
        SCK = 1;
        delay_us(5);
        if (MISO) Buff |= 1;
        SCK = 0;
    }
    return (Buff);
}

unsigned char SPI_Read(void) {
    unsigned char Buff = 0;
    unsigned char bit_ctr;
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        delay_us(5);
        Buff = (Buff << 1);
        SCK = 1;
        delay_us(5);
        Buff |= MISO;
        SCK = 0;
    }
    return (Buff);
}

void RF_Init() {
    CE = 1; delay_us(700); CE = 0; CSN = 1;
}

void RF_Command(unsigned char command) {
    CSN = 0; SPI_RW(command); CSN = 1;
    delay_us(10);
}

void RF_Write(char Reg_Add, char Value) {
    CSN = 0; SPI_RW(0x20 | Reg_Add); SPI_RW(Value);
    CSN = 1; delay_us(10);
}

void RF_Write_Address(unsigned char Address) {
    CSN = 0;
    RF_Write(0x03, 0x03);
    CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0x20 | 0x0A);
    SPI_RW(Address);
    SPI_RW(Address);
    SPI_RW(Address);
    SPI_RW(Address);
    CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0x20 | 0x10);
    SPI_RW(Address);
    SPI_RW(Address);
    SPI_RW(Address);
    SPI_RW(Address);
    CSN = 1; delay_us(10);
}

void RF_Config_Common(void) {
    CE = 0; CSN = 1; SCK = 0; delay_us(10);
    RF_Write(0x07, 0x70);
    RF_Command(0xE2);
    RF_Command(0xE1);
    RF_Write(0x11, 0x01);
    RF_Write(0x05, 0x02);
    RF_Write_Address(P_Add);
    RF_Write(0x02, 0x01);
    RF_Write(0x01, 0x00);
    RF_Write(0x04, 0x00);
}

void TX_Mode_Active(void) {
    CE = 0; RF_Write(0x00, 0x1E); delay_ms(2);
}

void RF_TX_send(char RX_Address, char Value) {
    RF_Write_Address(RX_Address);
    CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0xE1);
    CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0xA0);
    SPI_RW(Value);
    CSN = 1; CE = 1; delay_us(500); CE = 0;
    RF_Write(0x07, 0x7E);
    RF_Write_Address(P_Add);
}

void RX_Mode_Active(void) {
    CE = 0; RF_Write(0x00, 0x1F); CE = 1;
    delay_ms(2);
}

unsigned char RF_RX_Read() {
    unsigned char dem_RX;
    CE = 0; CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0x61);
    delay_us(10);
    dem_RX = SPI_Read();
    CSN = 1; CE = 1;
    RF_Write(0x07, 0x7E);
    RF_Command(0xE2);
    return dem_RX;
}

void main(void) {
    GLCDINIT_t glcd_init_data;
    unsigned char r;
    char str_buf[2] = {0, 0};
    DDRD = 0x80; PORTD = 0x80; DDRE = 0x38;
    PORTE = 0x08; DDRB = 0x08; PORTB = 0x05;
    glcd_init_data.font = font5x7;
    glcd_init_data.temp_coef = 63;
    glcd_init_data.bias = 4;
    glcd_init_data.vlcd = 50;
    glcd_init(&glcd_init_data);
    glcd_clear();
    glcd_outtextxy(0, 0, "SYSTEM READY");
    RF_Init();
    RF_Config_Common();
    while (1) {
        if (BT3 == 0) {
            delay_ms(200);
            if (mode != 1) {
                mode = 1;
                TX_Mode_Active();
                glcd_clear();
                glcd_outtextxy(0, 0, "MODE: SEND '8'");
            }
            else { 
                mode = 2;
                RX_Mode_Active();
                glcd_clear();
                glcd_outtextxy(0, 0, "MODE: RECEIVE");
            }
            while (BT3 == 0);
        }
        if (mode == 1) {
            RF_TX_send(P_Add, '8');
            delay_ms(500);
        }
        if (mode == 2) {
            if (IRQ == 0) {
                r = RF_RX_Read();
                str_buf[0] = r;
                glcd_outtextxy(x_pos, y_pos, str_buf);
                x_pos += 6;
                if (x_pos > 78) {
                    x_pos = 0; y_pos += 8;
                    if (y_pos > 40) {
                        glcd_clear();
                        glcd_outtextxy(0, 0, "MODE: RECEIVE");
                        x_pos = 0; y_pos = 10;
                    }
                }
            }
        }
    }
}