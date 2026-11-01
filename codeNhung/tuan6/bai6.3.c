// He thong khoa dien tu phan quyen 3 cap (Admin / Moderator / User)
// - Nhap 4 so tu ban phim ma tran 3x3, moi so hien 1s roi doi '*'
// - Mat khau luu trong EEPROM, neu EEPROM trong/hong thi nap mac dinh
// - Dung Admin -> bat ca 4 relay
// - Dung Mod   -> chi bat relay 3
// - Dung User  -> chi bat relay 4
// - Sai        -> tat het relay, bao ACCESS DENIED

#include <io.h>
#include <alcd.h>
#include <delay.h>
#include <stdio.h>
#include <string.h>

// Relay dau PORTC (thu tu nguoc de de nho: RELAY1 = PC0)
#define RELAY4 PORTC.3
#define RELAY3 PORTC.2
#define RELAY2 PORTC.1
#define RELAY1 PORTC.0
#define LCD_BL PORTD.7

int keypad[3][3] = {
    {1, 2, 3}, {4, 5, 6}, {7, 8, 9}
};

// Mat khau mac dinh cho 3 cap
const char DEF_PASS[3][5] = {
    "8888", "1234", "5555"
};

// Ban sao luu trong EEPROM (giu khi mat dien)
eeprom char EE_ADMIN[5];
eeprom char EE_MOD[5];
eeprom char EE_USER[5];

// Ban chay trong RAM de so sanh cho nhanh
char PASS_ADMIN[5];
char PASS_MOD[5];
char PASS_USER[5];
char input[5];      // chuoi that nguoi dung nhap
char disp[5];       // chuoi hien thi (so -> '*')
unsigned char idx = 0;

// Kiem tra ky tu co phai so 0-9 khong (dung de validate EEPROM)
char is_digit(char c) {
    return (c >= '0' && c <= '9');
}

void relays_off(void) {
    RELAY1 = 0; RELAY2 = 0; RELAY3 = 0; RELAY4 = 0;
}

void apply_admin(void) {
    RELAY1 = 1; RELAY2 = 1; RELAY3 = 1; RELAY4 = 1;
}

void apply_moderator(void) {
    relays_off(); RELAY3 = 1;
}

void apply_user(void) {
    relays_off(); RELAY4 = 1;
}

// Man hinh nhap mat khau
void show_pass_screen(void) {
    lcd_clear(); lcd_gotoxy(0, 0);
    lcd_puts("ENTER PASS:");
    lcd_gotoxy(0, 1);
    lcd_puts(disp);
}

// Reset buffer nhap, quay ve man hinh cho
void reset_pass(void) {
    unsigned char i;
    for (i = 0; i < 4; i++) {
        input[i] = '\0';
        disp[i] = ' ';
    }
    input[4] = '\0'; disp[4] = '\0';
    idx = 0;
    show_pass_screen();
}

// Doc mat khau tu EEPROM. Neu du lieu khong hop le thi ghi lai mac dinh.
void load_passwords_from_eeprom(void) {
    unsigned char i, valid;

    // Admin
    for (i = 0; i < 5; i++) PASS_ADMIN[i] = EE_ADMIN[i];
    valid = 1;
    for (i = 0; i < 4; i++) if (!is_digit(PASS_ADMIN[i])) valid = 0;
    if (!valid || PASS_ADMIN[4] != '\0') {
        strcpy(PASS_ADMIN, DEF_PASS[0]);
        for (i = 0; i < 5; i++) EE_ADMIN[i] = DEF_PASS[0][i];
    }

    // Moderator
    for (i = 0; i < 5; i++) PASS_MOD[i] = EE_MOD[i];
    valid = 1;
    for (i = 0; i < 4; i++) if (!is_digit(PASS_MOD[i])) valid = 0;
    if (!valid || PASS_MOD[4] != '\0') {
        strcpy(PASS_MOD, DEF_PASS[1]);
        for (i = 0; i < 5; i++) EE_MOD[i] = DEF_PASS[1][i];
    }

    // User
    for (i = 0; i < 5; i++) PASS_USER[i] = EE_USER[i];
    valid = 1;
    for (i = 0; i < 4; i++) if (!is_digit(PASS_USER[i])) valid = 0;
    if (!valid || PASS_USER[4] != '\0') {
        strcpy(PASS_USER, DEF_PASS[2]);
        for (i = 0; i < 5; i++) EE_USER[i] = DEF_PASS[2][i];
    }
}

// Quet ban phim, tra ve ky tu so vua nhan (hoac 0 neu khong co)
char scan_digit(void) {
    unsigned char r;
    int key;
    char d;

    for (r = 0; r < 3; r++) {
        // Chon hang r (giu nguyen cac chan khac tren PORTC/PORTD)
        if (r == 0) {
            PORTC = (PORTC & 0x0F) | 0x30;
            PORTE = 0x03;
            PORTD = 0x81;
        }
        if (r == 1) {
            PORTC = (PORTC & 0x0F) | 0x70;
            PORTE = 0x01;
            PORTD = 0x81;
        }
        if (r == 2) {
            PORTC = (PORTC & 0x0F) | 0x70;
            PORTE = 0x03;
            PORTD = 0x80;
        }
        delay_us(10);   // cho day on dinh

        // Cot 1 -> PC4
        if ((PINC & 0x10) == 0) {
            delay_ms(20);
            if ((PINC & 0x10) == 0) {
                key = keypad[r][0];
                d = (char)('0' + key);
                while ((PINC & 0x10) == 0) delay_ms(10);
                return d;
            }
        }
        // Cot 2 -> PC5
        if ((PINC & 0x20) == 0) {
            delay_ms(20);
            if ((PINC & 0x20) == 0) {
                key = keypad[r][1];
                d = (char)('0' + key);
                while ((PINC & 0x20) == 0) delay_ms(10);
                return d;
            }
        }
        // Cot 3 -> PE0
        if ((PINE & 0x01) == 0) {
            delay_ms(20);
            if ((PINE & 0x01) == 0) {
                key = keypad[r][2];
                d = (char)('0' + key);
                while ((PINE & 0x01) == 0) delay_ms(10);
                return d;
            }
        }
    }
    return 0;   // khong co phim nao duoc nhan
}

// Xu ly 1 ky tu vua nhap: hien so 1s -> doi '*' -> kiem tra khi du 4 so
void handle_digit(char d) {
    input[idx] = d;
    disp[idx] = d;
    show_pass_screen();
    delay_ms(1000);            // hien so trong 1 giay

    disp[idx] = '*';           // che mat khau
    show_pass_screen();
    idx++;

    if (idx >= 4) {            // du 4 ky tu -> so sanh
        if (strcmp(input, PASS_ADMIN) == 0) {
            apply_admin();
            lcd_clear();
            lcd_puts("ACCESS GRANTED");
            lcd_gotoxy(0, 1); lcd_puts("ADMIN");
            delay_ms(1500);
        }
        else if (strcmp(input, PASS_MOD) == 0) {
            apply_moderator();
            lcd_clear();
            lcd_puts("ACCESS GRANTED");
            lcd_gotoxy(0, 1); lcd_puts("MODERATOR");
            delay_ms(1500);
        }
        else if (strcmp(input, PASS_USER) == 0) {
            apply_user();
            lcd_clear();
            lcd_puts("ACCESS GRANTED");
            lcd_gotoxy(0, 1); lcd_puts("USER");
            delay_ms(1500);
        }
        else {
            relays_off();
            lcd_clear();
            lcd_puts("ACCESS DENIED");
            delay_ms(1200);
        }
        relays_off();          // xu ly xong thi tat het relay
        reset_pass();          // quay ve man hinh nhap
    }
}

void main(void) {
    lcd_init(16);

    // Cau hinh chan:
    //   PORTC: PC0-PC3 = relay (output), PC6 = den nen, PC4/PC5 = input cot
    //   PORTE: PE1 = output hang, PE0 = input cot
    //   PORTD: PD0 = output hang, PD7 = den nen, cac chan LED khac
    DDRC = 0x4F; PORTC = 0x70;   // bat pull-up cho PC4, PC5
    DDRE = 0x02; PORTE = 0x03;   // bat pull-up PE0
    DDRD = 0x87; PORTD = 0x81;
    LCD_BL = 1;

    relays_off();
    load_passwords_from_eeprom();
    reset_pass();

    while (1) {
        char d = scan_digit();
        if (d) handle_digit(d);
    }
}