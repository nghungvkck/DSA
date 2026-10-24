// logic
// bấm BT-1 thì led -g sáng, bấm bt-1 lần 2 thì Led-g tắt, lặp lại quá trình này ở các nút khác

#include <mega128.h>
#include <delay.h>

// Định nghĩa tên các nút bấm kết nối với chân PINB.2, PINB.3, PINB.0
#define BT_1    PINB.2
#define BT_2    PINB.3
#define BT_3    PINB.0

#define LED_R   PORTD.4
#define LED_G   PORTD.5
#define LED_B   PORTD.6

void io_init(void) {
    // DDRD = 0b01110000: Cấu hình 3 chân PD4, PD5, PD6 là ngõ ra (Output) để điều khiển LED. Các chân khác là ngõ vào.
    // PORTD = 0b00000000: Đặt mức điện áp ban đầu cho cổng D là 0V (tắt tất cả LED ban đầu)
    // DDRB = 0x00: Cấu hình toàn bộ cổng B là ngõ vào (Input) để đọc trạng thái các nút bấm.
    // PORTB = 0x0D: Bật điện trở kéo lên (Pull-up) nội bộ cho các chân PB0, PB2, PB3. 
    // Giúp khi không bấm nút, chân sẽ ở mức 1 (HIGH), khi bấm xuống mass sẽ là 0 (LOW).
    DDRD = 0b01110000; 
    PORTD = 0b00000000;
    DDRB = 0x00; 
    PORTB = 0x0D;
}

// hàm tắt tất cả các led
void leds_off(void) {
    LED_R = 0; 
    LED_G = 0; 
    LED_B = 0;
}
// Hàm chính
void main(void) {
    io_init(); 
    
    while (1) { 
        
        if (BT_1 == 0) { 
            leds_off();     // Tắt hết các LED khác
            LED_G = 1;      // Bật LED Xanh lá (Green)
        }

        else if (BT_2 == 0) { 
            leds_off();     // Tắt hết các LED khác
            LED_B = 1;      // Bật LED Xanh dương (Blue)
        }

        else if (BT_3 == 0) { 
            leds_off();     // Tắt hết các LED khác
            LED_R = 1;      // Bật LED Đỏ (Red)
        }
        
        else { 
            leds_off();     // Tắt tất cả các LED
        }
        delay_ms(10);
    }
}