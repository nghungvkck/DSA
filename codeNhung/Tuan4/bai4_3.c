// 1 nut nhan (BT1 - PB2) luan phien 3 che do:
//   1. Chu "Hello-World" roi tu phai sang trai
//   2. Chu "Hello-World" roi tu trai sang phai
//   3. Xoa man hinh

#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <string.h>

#define BT1 PINB.2

unsigned char bt1_count = 0;

void main(void) {
    char text[] = "Hello-World";
    int len = strlen(text);
    int i, j, k, start_pos, land_pos;

    DDRB  = 0x00;
    PORTB = 0x04;               // pull-up cho PB2
    DDRA  = 0xFF;
    DDRD  = 0x80;
    PORTD = 0x80;               // bat den nen LCD

    lcd_init(16);
    lcd_clear();

    while (1) {
        if (BT1 == 0) {
            delay_ms(50);                // chong doi
            if (BT1 == 0) {

                bt1_count++;
                if (bt1_count > 3) bt1_count = 1;

                switch (bt1_count) {
                    case 1:                  // roi tu phai sang trai
                        lcd_clear();
                        start_pos = 16 - len;   // chuoi chiem cot 5..15

                        for (i = len - 1; i >= 0; i--) {       // duyet tu cuoi len dau
                            land_pos = start_pos + i;

                            for (j = 0; j <= land_pos; j++) {  // roi tu cot 0 -> land_pos
                                lcd_clear();

                                // Ve cac ky tu da ha canh
                                for (k = i + 1; k < len; k++) {
                                    lcd_gotoxy(start_pos + k, 0);
                                    lcd_putchar(text[k]);
                                }

                                lcd_gotoxy(j, 0);              // ve ky tu dang roi
                                lcd_putchar(text[i]);

                                delay_ms(30);
                            }
                        }
                        break;

                    case 2:                  // roi tu trai sang phai
                        lcd_clear();
                        for (i = 0; i < len; i++) {           // duyet tu dau den cuoi
                            for (j = 15; j >= i; j--) {       // roi tu cot 15 ve cot i
                                lcd_clear();

                                // Ve cac ky tu da ha canh
                                for (k = 0; k < i; k++) {
                                    lcd_gotoxy(k, 0);
                                    lcd_putchar(text[k]);
                                }

                                lcd_gotoxy(j, 0);              // ve ky tu dang roi
                                lcd_putchar(text[i]);

                                delay_ms(30);
                            }
                        }
                        break;

                    case 3:
                        lcd_clear();
                        break;
                }

                while (BT1 == 0);   // cho nha nut
            }
        }
    }
}