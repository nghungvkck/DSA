// Khoa dien tu dung ban phim ma tran 3x3
// Nhap mat khau 6 so, moi so hien 1s roi doi thanh '*'
// Mat khau dung: "123123" -> mo relay 1
// Sai 3 lan -> khoa, bat relay 2 + nhay LED xanh/do canh bao

#include <alcd.h>
#include <delay.h>
#include <stdio.h>
#include <string.h>

#define RELAY1 PORTC.3
#define RELAY2 PORTC.2
#define LED_R  PORTD.4
#define LED_B  PORTD.5
#define LCD_BL PORTD.7

int keypad[3][3] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9}
};

char data[16];
char PASS[7] = "123123";      // mat khau dung
char input[7], disp[7];       // input: chuoi that, disp: chuoi hien thi
unsigned char idx = 0;        // so ky tu da nhap
unsigned char fail_count = 0; // so lan sai
char unlocked = 0;            // 1 = da mo khoa

// Ve lai man hinh nhap mat khau
void show_pass(void) {
    lcd_clear(); lcd_gotoxy(0, 0);
    lcd_puts("PASS: ");
    lcd_gotoxy(0, 1);
    lcd_puts(disp);
}

// Xoa buffer, quay ve trang thai cho nhap
void reset_pass(void) {
    unsigned char i;
    for (i = 0; i < 6; i++) {
        input[i] = '\0';
        disp[i] = ' ';
    }
    input[6] = '\0'; disp[6] = '\0';
    idx = 0;
    show_pass();
}

// Che do canh bao: khoa cung, nhay LED luan phien mai mai
void alarm_mode(void) {
    RELAY1 = 0; RELAY2 = 1;         // ngat relay mo, dong relay canh bao
    lcd_clear();
    lcd_puts("ALARM! LOCKED");
    while (1) {
        LED_B = 1; LED_R = 0; delay_ms(200);
        LED_B = 0; LED_R = 1; delay_ms(200);
    }
}

// Xu ly khi co 1 phim so duoc nhan
void handle_digit(char d) {
    // Hien so vua nhan 1s roi che thanh '*'
    input[idx] = d;
    disp[idx] = d;
    show_pass();
    delay_ms(500);                  // hien so 0.5s
    disp[idx] = '*';
    show_pass();
    idx++;

    // Du 6 so -> kiem tra mat khau
    if (idx >= 6) {
        if (strcmp(input, PASS) == 0) {
            // Dung -> mo khoa
            unlocked = 1;
            RELAY1 = 1; RELAY2 = 0;
            LED_B = 0; LED_R = 0;
            lcd_clear();
            lcd_puts("SUCCESS");
            lcd_gotoxy(0, 1);
            lcd_puts("RELAY1 ON");
        } else {
            // Sai -> tang bien dem
            fail_count++;
            RELAY1 = 0;
            lcd_clear();
            sprintf(data, "WRONG: %d/3", fail_count);
            lcd_puts(data);
            delay_ms(800);
            if (fail_count >= 3) alarm_mode();   // sai 3 lan -> khoa
            reset_pass();                        // chua du 3 lan -> cho nhap lai
        }
    }
}

// Quet ban phim ma tran 3x3 (quet hang, doc cot)
void button(void) {
    unsigned char r;
    int key;
    char d;

    for (r = 0; r < 3; r++) {
        // Chon hang r (keo hang r xuong 0), giu nguyen cac chan khac
        if (r == 0) {
            PORTC = (PORTC & 0x0C) | 0x30;
            PORTE = 0x03;
            PORTD = (PORTD & 0xD0) | 0x01;
        }
        if (r == 1) {
            PORTC = (PORTC & 0x0C) | 0x70;
            PORTE = 0x01;
            PORTD = (PORTD & 0xD0) | 0x01;
        }
        if (r == 2) {
            PORTC = (PORTC & 0x0C) | 0x70;
            PORTE = 0x03;
            PORTD = (PORTD & 0xD0) | 0x00;
        }
        delay_us(10);   // cho duong day on dinh

        // Doc cot 1 (PC4)
        if ((PINC & 0x10) == 0) {
            delay_ms(20);
            if ((PINC & 0x10) == 0) {
                key = keypad[r][0];
                d = (char)('0' + key);
                while ((PINC & 0x10) == 0) delay_ms(10);  // cho nha phim
                handle_digit(d);
                return;
            }
        }

        // Doc cot 2 (PC5)
        if ((PINC & 0x20) == 0) {
            delay_ms(20);
            if ((PINC & 0x20) == 0) {
                key = keypad[r][1];
                d = (char)('0' + key);
                while ((PINC & 0x20) == 0) delay_ms(10);
                handle_digit(d);
                return;
            }
        }

        // Doc cot 3 (PE0)
        if ((PINE & 0x01) == 0) {
            delay_ms(20);
            if ((PINE & 0x01) == 0) {
                key = keypad[r][2];
                d = (char)('0' + key);
                while ((PINE & 0x01) == 0) delay_ms(10);
                handle_digit(d);
                return;
            }
        }
    }
}

void main(void) {
    lcd_init(16);

    // Cau hinh chan:
    //   PC6 = output (den nen LCD), PC2/PC3 = relay, PC4/PC5 = input cot
    //   PE1 = output (hang), PE0 = input cot
    //   PD0 = output (hang), PD4/PD5 = LED, PD7 = den nen
    DDRC = 0x4C;  PORTC = 0x70;   // bat pull-up PC4, PC5
    DDRE = 0x02;  PORTE = 0x03;   // pull-up PE0
    DDRD = 0xD1;  PORTD = 0x81;
    RELAY1 = 0; RELAY2 = 0;
    LED_R = 0;  LED_B = 0;
    LCD_BL = 1;                   // bat den nen

    reset_pass();                 // hien man hinh nhap ban dau

    while (1) {
        if (!unlocked) button();  // da mo khoa thi khong quet phim nua
    }
}