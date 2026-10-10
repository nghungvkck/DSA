#include <io.h>
#include <mega128.h>
#include <delay.h>
#include <alcd.h>
#include <stdio.h>

#define CE PORTB.3
#define CSN PORTE.3
#define SCK PORTE.4
#define MOSI PORTE.5
#define MISO PINE.2
#define IRQ PINB.2
#define BT3 PINB.0
#define RELAY1 PORTC.3
#define RELAY2 PORTC.2
#define RELAY3 PORTC.1
#define RELAY4 PORTC.0
#define LED_R PORTD.4
#define LED_B PORTD.6

unsigned char P_Add = 0xB5;
unsigned char mode = 0;
unsigned char col_pos = 0;
unsigned char last_k = 0xFF;

unsigned char SPI_RW(unsigned char Buff) {
    unsigned char bit_ctr;
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        MOSI = (Buff & 0x80); delay_us(5);
        Buff = (Buff << 1); SCK = 1; delay_us(5);
        if (MISO) Buff |= 1; SCK = 0;
    }
    return (Buff);
}

unsigned char SPI_Read(void) {
    unsigned char Buff = 0;
    unsigned char bit_ctr;
    for (bit_ctr = 0; bit_ctr < 8; bit_ctr++) {
        delay_us(5); Buff = (Buff << 1);
        SCK = 1; delay_us(5); Buff |= MISO; SCK = 0;
    }
    return (Buff);
}

void RF_Init() {
    CE = 1; delay_us(700); CE = 0; CSN = 1;
}

void RF_Command(unsigned char command) {
    CSN = 0; SPI_RW(command); CSN = 1; delay_us(10);
}

void RF_Write(char Reg_Add, char Value) {
    CSN = 0; SPI_RW(0x20 | Reg_Add); SPI_RW(Value);
    CSN = 1; delay_us(10);
}

void RF_Write_Address(unsigned char Address) {
    CSN = 0; RF_Write(0x03, 0x03);
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
    CE = 0; RF_Write(0x00, 0x1F); CE = 1; delay_ms(2);
}

unsigned char RF_RX_Read() {
    unsigned char dem_RX;
    CE = 0; CSN = 1; delay_us(10); CSN = 0;
    SPI_RW(0x61); delay_us(10);
    dem_RX = SPI_Read();
    CSN = 1; CE = 1;
    RF_Write(0x07, 0x7E);
    RF_Command(0xE2);
    return dem_RX;
}

unsigned char Scan_Keypad_3x3(void) {
    unsigned char key = 0xFF;
    DDRC = (DDRC & 0x9F) | 0x30;
    DDRE = (DDRE & 0xFD) | 0x01;
    DDRD = DDRD & 0xFE;
    PORTC |= 0x40; PORTE &= ~0x02; PORTD |= 0x01;
    delay_us(10);
    if ((PINC & 0x40) == 0) key = '1';
    else if ((PINE & 0x02) == 0) key = '4';
    else if ((PIND & 0x01) == 0) key = '7';
    PORTC |= 0x30; PORTE &= 0xFE; delay_us(10);
    if ((PINC & 0x40) == 0) key = '2';
    else if ((PINE & 0x02) == 0) key = '5';
    else if ((PIND & 0x01) == 0) key = '8';
    PORTC |= 0x30; PORTE &= 0xFE; delay_us(10);
    if ((PINC & 0x40) == 0) key = '3';
    else if ((PINE & 0x02) == 0) key = '6';
    else if ((PIND & 0x01) == 0) key = '9';
    PORTC |= 0x30;
    PORTE |= 0x01;
    return key;
}

void main(void) {
    unsigned char r, k;
    DDRE = 0x38;
    DDRD = 0xCD; PORTD = 0x90;
    PORTE = 0x08; DDRB = 0x08;
    PORTB = 0x05; DDRC = 0x0F;
    lcd_init(16);
    lcd_clear();
    lcd_puts("SYSTEM READY");
    
    RF_Init();
    RF_Config_Common();
    
    while (1) {
        if (BT3 == 0) {
            delay_ms(200);
            if (mode != 1) {
                mode = 1;
                TX_Mode_Active();
                lcd_clear(); lcd_puts("MODE: SEND");
            }
            else {
                mode = 2;
                RX_Mode_Active();
                lcd_clear(); lcd_puts("MODE: RECEIVE");
                lcd_gotoxy(0, 1);
                col_pos = 0;
            }
            while (BT3 == 0);
        }
        
        if (mode == 1) {
            k = Scan_Keypad_3x3();
            if (k != 0xFF) {
                if (k != last_k) {
                    RF_TX_send(P_Add, k);
                    lcd_gotoxy(0, 1); lcd_puts("Sent Key:   ");
                    lcd_putchar(k);
                    last_k = k;
                }
            }
            else {
                RF_TX_send(P_Add, 0);
                last_k = 0xFF;
                lcd_gotoxy(0, 1); lcd_puts("Sending 0");
                delay_ms(100);
            }
        }
        
        if (mode == 2) {
            if (IRQ == 0) {
                LED_R = !LED_R; LED_B = !LED_R;
                r = RF_RX_Read();
                if (r != 0 && r != 0xFF) {
                    lcd_gotoxy(col_pos, 1); lcd_putchar(r);
                    col_pos++;
                    if (col_pos >= 16) {
                        lcd_gotoxy(0, 1); lcd_puts("                ");
                        col_pos = 0;
                    }
                    switch (r) {
                        case '1': RELAY1 = !RELAY1; break;
                        case '2': RELAY2 = !RELAY2; break;
                        case '3': RELAY3 = !RELAY3; break;
                        case '4': RELAY4 = !RELAY4; break;
                        default: break;
                    }
                }
                RF_Write(0x07, 0x40);
            }
        }
    }
}