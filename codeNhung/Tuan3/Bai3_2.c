// logic là bấm nút 1 thì đếm tăng và hiển thị
// bấm nút 2 thì đếm giảm va hiển thị
// bấm 3 thì đếm về 0

#include <mega128.h>
#include <delay.h>

int count = 0;

// Hàm hiển thị 1 chữ số lên LED 7 thanh
// i: số cần hiển thị (0-9)
// select_mask: mặt nạ chọn vị trí LED (để quét LED)
void hienThi(int i, unsigned char select_mask) {
    PORTF &= 0x00; 
    PORTA &= 0x7E; 
    PORTD &= 0xF1;
    
    if (i == 0) { PORTF = 0x26 | select_mask; 
                  PORTA = 0x80; PORTD = 0x06; }
    if (i == 1) { PORTF = 0x20 | select_mask; 
                  PORTA = 0x80; PORTD = 0x00; }
    if (i == 2) { PORTF = 0x22 | select_mask; 
                  PORTA = 0x00; PORTD = 0x0E; }
    if (i == 3) { PORTF = 0x22 | select_mask; 
                  PORTA = 0x80; PORTD = 0x0A; }
    if (i == 4) { PORTF = 0x24 | select_mask; 
                  PORTA = 0x80; PORTD = 0x08; }
    if (i == 5) { PORTF = 0x06 | select_mask; 
                  PORTA = 0x80; PORTD = 0x0A; }
    if (i == 6) { PORTF = 0x06 | select_mask; 
                  PORTA = 0x80; PORTD = 0x0E; }
    if (i == 7) { PORTF = 0x22 | select_mask; 
                  PORTA = 0x80; PORTD = 0x00; }
    if (i == 8) { PORTF = 0x26 | select_mask; 
                  PORTA = 0x80; PORTD = 0x0E; }
    if (i == 9) { PORTF = 0x26 | select_mask; 
                  PORTA = 0x80; PORTD = 0x0A; }
}

// Hàm quét hiển thị liên tục 2 chữ số
void refreshDisplay() {
    hienThi(count / 10, 0x58); delay_ms(5); // Hiển thị hàng chục
    hienThi(count % 10, 0x51); delay_ms(5); // Hiển thị hàng đơn vị
}

void main() {
    DDRF = 0xFF; 
    DDRD = 0xFF; 
    DDRA = 0xFF;
    
    DDRB = 0x00; 
    PORTB = 0x0D;
    
    while (1) {
        // Nút bấm BT1 (PINB.2) - Tăng số
        if (PINB.2 == 0) { 
            delay_ms(20); // Chống dội phím
            if (PINB.2 == 0) {
                if (count < 99) count++;
                while (PINB.2 == 0) refreshDisplay(); // Giữ nút vẫn quét LED để không bị mất số
            }
        }
        
        // Nút bấm BT2 (PINB.3) - Giảm số
        if (PINB.3 == 0) { 
            delay_ms(20);
            if (PINB.3 == 0) {
                if (count > 0) count--;
                while (PINB.3 == 0) refreshDisplay();
            }
        }
        
        // Nút bấm BT3 (PINB.0) - Reset về 0
        if (PINB.0 == 0) { 
            delay_ms(20);
            if (PINB.0 == 0) {
                count = 0;
                while (PINB.0 == 0) refreshDisplay();
            }
        }
        
        // Luôn quét hiển thị khi không bấm nút
        refreshDisplay(); 
    }
}