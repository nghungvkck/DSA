// logic, bam 1 hien thi 1
// bam 2 hien thi 2
// bam 3 hien thi 3
// Danh cho CodeVisionAVR - LED 7 thanh ANODE CHUNG (Muc 0 sang)

#include <io.h>
#include <delay.h>

int mode = 0; // 0=Tat, 1=Hien so 1, 2=Hien so 2, 3=Hien so 3

// Ham xuat du lieu ra LED 7 thanh
void hienthi(int i, unsigned char select_mask) {
    unsigned char f_data;

    // Xoa du lieu cu tranh bong mo
    PORTA = 0xFF;
    PORTD = 0xFF;
    f_data = 0xFF;

    // Ma hien thi Anode chung (0 = sang)
    if (i == 1) { f_data = 0xDF; PORTA = 0x7F; PORTD = 0xFF; } // So 1: doan b, c sang
    if (i == 2) { f_data = 0xDD; PORTA = 0xFF; PORTD = 0xF1; } // So 2: doan a, b, d, e, g sang
    if (i == 3) { f_data = 0xDD; PORTA = 0x7F; PORTD = 0xF5; } // So 3: doan a, b, c, d, g sang

    // Xuat ra PORTF chon LED sang
    PORTF = f_data & select_mask;
}

// Cap nhat hien thi theo mode
void updateDisplay() {
    if (mode == 1) hienthi(1, 0x58);
    else if (mode == 2) hienthi(2, 0x51);
    else if (mode == 3) hienthi(3, 0x49);
    else {
        // Mode 0: tat het
        PORTF = 0xFF;
        PORTA = 0xFF;
        PORTD = 0xFF;
    }
}

void main() {
    // Cau hinh chan: A, D, F la Output; B la Input
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;
    DDRB = 0x00;
    PORTB = 0x0D; // Bat tro keo len cho nut bam

    // Tat het LED ban dau
    PORTF = 0xFF;
    PORTA = 0xFF;
    PORTD = 0xFF;

    while (1) {
        // Nut BT1 chon mode 1
        if (PINB.2 == 0) {
            delay_ms(20); // Chong doi
            if (PINB.2 == 0) {
                mode = 1;
                while (PINB.2 == 0) updateDisplay();
            }
        }

        // Nut BT2 chon mode 2
        if (PINB.3 == 0) {
            delay_ms(20);
            if (PINB.3 == 0) {
                mode = 2;
                while (PINB.3 == 0) updateDisplay();
            }
        }

        // Nut BT3 chon mode 3
        if (PINB.0 == 0) {
            delay_ms(20);
            if (PINB.0 == 0) {
                mode = 3;
                while (PINB.0 == 0) updateDisplay();
            }
        }

        // Duy tri hien thi lien tuc
        updateDisplay();
    }
}