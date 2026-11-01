// logic, bam 1 hien thi 1
// bam 2 hien thi 2
// bam 3 hien thi 3

#include <io.h>
#include <delay.h>

int mode = 0; // 0=Tat, 1=Hien so 1, 2=Hien so 2, 3=Hien so 3

// Ham xuat du lieu ra LED
void hienthi(int i, unsigned char select_mask) {
    unsigned char f_data;
    PORTA = 0x00; PORTD = 0x00; f_data = 0x00; // Xoa du lieu cu tranh bong mo
    
    // Gan ma hien thi va ma chon LED tuy theo so i
    if (i == 1) { f_data = 0x20; PORTA = 0x80; PORTD = 0x00; }
    if (i == 2) { f_data = 0x22; PORTA = 0x00; PORTD = 0x0E; }
    if (i == 3) { f_data = 0x22; PORTA = 0x80; PORTD = 0x0A; }
    
    PORTF = f_data | select_mask; // Ket hop xuat ra PORTF chon LED sang
}

// Cap nhat hien thi theo mode
void updateDisplay() {
    if (mode == 1) hienthi(1, 0x58);
    else if (mode == 2) hienthi(2, 0x51);
    else if (mode == 3) hienthi(3, 0x49);
    else { PORTF = 0x59; PORTA = 0x00; PORTD = 0x00; } // Mode 0 tat het
}

void main() {
    // Cau hinh chan: A, D, F la Output; B la Input
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;
    DDRB = 0x00;
    PORTB = 0x0D; // Bat tro keo len cho nut bam
    
    while (1) {
        // Nut BT1 chon mode 1
        if (PINB.2 == 0) {
            delay_ms(20); // Chong doi
            if (PINB.2 == 0) {
                mode = 1;
                while (PINB.2 == 0) updateDisplay(); // Giu nut van hien thi
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