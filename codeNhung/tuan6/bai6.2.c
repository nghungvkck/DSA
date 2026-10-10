// ================================
// viết chương trình mô hình khóa điện tử dùng bàn phím ma trận 3x3 theo kịch bản
// người dùng nhập mật khẩu gồm 6 chữ số từ bàn phím
// mỗi lần nhấn phím, số vừa nhập được hiển thị tạm thời tren LCD, sau 1 giây tự đổng chuyển thành kí tự * để che mật khẩu
// mật khẩu hợp lệ được quy định là "108108"
// Nếu người dùng nhập đúng mật khẩu, hệ thống kích hoạt rolay ở trạng thái đóng để mở khóa, 
// Nếu người dùng nhập sai mật khẩu quá 3 lần, hệ thống kích hoạt rolay 2 ở trạng thái đóng, đồng thời led xanh- đỏ nhấp 
// nháy luôn phiên để phát cảnh báo
//=================================

#include <alcd.h>
#include <delay.h>
#include <stdio.h>
#include <string.h>

#define RELAY1 PORTC.3
#define RELAY2 PORTC.2
#define LED_R PORTD.4
#define LED_B PORTD.5
#define LCD_BL PORTD.7

int keypad[3][3] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9}
};

char data[16];
char PASS[7] = "123123";
char input[7], disp[7];
unsigned char idx = 0;
unsigned char fail_count = 0;
char unlocked = 0;

void show_pass(void) {
    lcd_clear(); lcd_gotoxy(0, 0);
    lcd_puts("PASS: "); lcd_gotoxy(0, 1);
    lcd_puts(disp);
}

void reset_pass(void) {
    unsigned char i;
    for (i = 0; i < 6; i++) {
        input[i] = '\0'; disp[i] = ' ';
    }
    input[6] = '\0'; disp[6] = '\0';
    idx = 0; show_pass();
}

void alarm_mode(void) {
    RELAY1 = 0; RELAY2 = 1; lcd_clear();
    lcd_puts("ALARM! LOCKED");
    while (1) {
        LED_B = 1; LED_R = 0; delay_ms(200);
        LED_B = 0; LED_R = 1; delay_ms(200);
    }
}

void handle_digit(char d) {
    input[idx] = d; disp[idx] = d; show_pass();
    delay_ms(500); disp[idx] = '*'; show_pass();
    idx++;
    if (idx >= 6) {
        if (strcmp(input, PASS) == 0) {
            unlocked = 1; RELAY1 = 1; RELAY2 = 0;
            LED_B = 0; LED_R = 0;
            lcd_clear(); lcd_puts("SUCCESS");
            lcd_gotoxy(0, 1); lcd_puts("RELAY1 ON");
        } else {
            fail_count++;
            RELAY1 = 0; lcd_clear();
            sprintf(data, "WRONG: %d/3", fail_count);
            lcd_puts(data); delay_ms(800);
            if (fail_count >= 3) alarm_mode();
            reset_pass();
        }
    }
}

void button(void) {
    unsigned char r;
    int key;
    char d;
    for (r = 0; r < 3; r++) {
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
        delay_us(10);
        
        // Kiểm tra cột 1 (PINC.4)
        if ((PINC & 0x10) == 0) { delay_ms(20); 
            if ((PINC & 0x10) == 0) {
                key = keypad[r][0];
                d = (char)('0' + key);
                while ((PINC & 0x10) == 0) delay_ms(10);
                handle_digit(d);
                return;
            }
        }
        
        // Kiểm tra cột 2 (PINC.5)
        if ((PINC & 0x20) == 0) { delay_ms(20);
            if ((PINC & 0x20) == 0) {
                key = keypad[r][1]; d = (char)('0' + key);
                while ((PINC & 0x20) == 0) delay_ms(10);
                handle_digit(d);
                return;
            }
        }
        
        // Kiểm tra cột 3 (PINE.0)
        if ((PINE & 0x01) == 0) { delay_ms(20);
            if ((PINE & 0x01) == 0) {
                key = keypad[r][2]; d = (char)('0' + key);
                while ((PINE & 0x01) == 0) delay_ms(10);
                handle_digit(d);
                return;
            }
        }
    }
}

void main(void) {
    lcd_init(16); DDRC = 0x4C; PORTC = 0x70;
    DDRE = 0x02; PORTE = 0x03; DDRD = 0xD1;
    PORTD = 0x81; RELAY1 = 0; RELAY2 = 0;
    LED_R = 0; LED_B = 0; LCD_BL = 1; reset_pass();
    while (1) {
        if (!unlocked)
            button();
    }
}