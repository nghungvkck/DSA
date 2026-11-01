// bấm bt1 - bt2: + thêm người 
// bấm bt2 - bt1: trừ người 
// nếu phòng có ít nhất 1 người thì bật relay 1
// nếu phòng ko có ai thì tắt relay
// hiện thị số người trên led 7 đoạn

#include <io.h>
#include <delay.h>

int count = 0;
int state = 0;

void hienthi(int i, unsigned char select_mask) {
    unsigned char f_data = 0x00;
    PORTA = 0x00; 
    PORTD = 0x00;
    
    if (i == 1) { 
        f_data = 0x20; PORTA = 0x80; PORTD = 0x00; 
    } else if (i == 2) { 
        f_data = 0x22; PORTA = 0x00; PORTD = 0x0E; 
    } else if (i == 3) { 
        f_data = 0x22; PORTA = 0x80; PORTD = 0x0A; 
    }
    
    PORTF = f_data | select_mask;
}

void updateDisplay() {
    if (count == 1)      hienthi(1, 0x58);
    else if (count == 2) hienthi(2, 0x51);
    else if (count == 3) hienthi(3, 0x49);
    else {
        PORTF = 0x59; 
        PORTA = 0x00; 
        PORTD = 0x00;
    }
}

void updateRelay() {
    if (count > 0) {
        PORTC |= (1 << 3);
    } else {
        PORTC &= ~(1 << 3);
    }
}

void main() {
    DDRF = 0xFF;
    DDRD = 0xFF;
    DDRA = 0xFF;
    DDRC = 0xFF;
    
    DDRB = 0x00;
    PORTB = 0x0D;

    PORTC = 0x00;

    while (1) {
        if (PINB.2 == 0) {
            delay_ms(20);
            if (PINB.2 == 0) {
                if (state == 0) {
                    state = 1;
                } else if (state == 2) {
                    if (count > 0) count--;
                    state = 0;
                }
                
                while (PINB.2 == 0) {
                    updateDisplay();
                    updateRelay();
                }
            }
        }

        if (PINB.3 == 0) {
            delay_ms(20);
            if (PINB.3 == 0) {
                if (state == 0) {
                    state = 2;
                } else if (state == 1) {
                    count++;
                    state = 0;
                }
                
                while (PINB.3 == 0) {
                    updateDisplay();
                    updateRelay();
                }
            }
        }

        updateRelay();
        updateDisplay();
    }
}