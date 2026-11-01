// Đếm từ 30 về 0 trên LED 7 đoạn (2 digit)
// Sử dụng PORTA, PORTD để điều khiển segment
// PORTF để chọn digit (multiplexing)

#include <io.h>
#include <delay.h>

int count = 30;

// Mã 7 đoạn cho số 0-9 (common cathode hoặc anode tùy mạch)
// Giả sử common cathode: bit=1 thì sáng
//    a
//   ---
// f|   |b
//   -g-
// e|   |c
//   ---
//    d
// Segment: g f e d c b a
unsigned char seg_code[10] = {
    0x3F, // 0: 0011 1111
    0x06, // 1: 0000 0110
    0x5B, // 2: 0101 1011
    0x4F, // 3: 0100 1111
    0x66, // 4: 0110 0110
    0x6D, // 5: 0110 1101
    0x7D, // 6: 0111 1101
    0x07, // 7: 0000 0111
    0x7F, // 8: 0111 1111
    0x6F  // 9: 0110 1111
};

// Hàm hiển thị 2 digit số n (0-99)
// Giả sử:
//   - Digit chục (hàng chục): PORTA điều khiển segment, PORTF chọn digit
//   - Digit đơn vị: PORTD điều khiển segment, PORTF chọn digit
// (giữ nguyên cấu trúc phần cứng của bạn)
void hienthi2so(int n) {
    int chuc = n / 10;
    int donvi = n % 10;
    
    // Hiển thị hàng chục
    PORTA = seg_code[chuc];
    PORTD = 0x00;
    PORTF = 0x01;  // chọn digit chục (bit0 = 1)
    delay_ms(5);
    
    // Hiển thị hàng đơn vị
    PORTA = 0x00;
    PORTD = seg_code[donvi];
    PORTF = 0x02;  // chọn digit đơn vị (bit1 = 1)
    delay_ms(5);
}

void main() {
    DDRF = 0xFF;   // PORTF output (chọn digit)
    DDRD = 0xFF;   // PORTD output (segment digit đơn vị)
    DDRA = 0xFF;   // PORTA output (segment digit chục)
    DDRC = 0xFF;   // PORTC output (relay nếu cần)
    
    DDRB = 0x00;   // PORTB input (nút nhấn)
    PORTB = 0xFF;  // Pull-up
    
    PORTC = 0x00;
    
    while (1) {
        // Đếm từ 30 về 0
        for (count = 30; count >= 0; count--) {
            // Hiển thị số trong ~1 giây (multiplexing liên tục)
            // 1 giây / 10ms mỗi lần quét = 100 lần
            int i;
            for (i = 0; i < 100; i++) {
                hienthi2so(count);
            }
        }
        
        // Sau khi về 0, có thể dừng hoặc lặp lại
        // Nếu muốn lặp lại, giữ nguyên vòng while
        // Nếu muốn dừng ở 0, thêm while(1) ở đây
        
        // delay trước khi lặp lại
        delay_ms(500);
    }
}