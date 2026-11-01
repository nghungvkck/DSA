// logic
// bam BT-1 thi led-g sang, bam bt-1 lan 2 thi Led-g tat, lap lai qua trinh nay o cac nut khac

#include <mega128.h>
#include <delay.h>

// Dinh nghia ten cac nut bam ket noi voi chan PINB.2, PINB.3, PINB.0
#define BT_1    PINB.2
#define BT_2    PINB.3
#define BT_3    PINB.0

#define LED_R   PORTD.4
#define LED_G   PORTD.5
#define LED_B   PORTD.6

void io_init(void) {
    // DDRD = 0b01110000: Cau hinh 3 chan PD4, PD5, PD6 la ngo ra (Output) de dieu khien LED. Cac chan khac la ngo vao.
    // PORTD = 0b00000000: Dat muc dien ap ban dau cho cong D la 0V (tat tat ca LED ban dau)
    // DDRB = 0x00: Cau hinh toan bo cong B la ngo vao (Input) de doc trang thai cac nut bam.
    // PORTB = 0x0D: Bat dien tro keo len (Pull-up) noi bo cho cac chan PB0, PB2, PB3.
    // Giup khi khong bam nut, chan se o muc 1 (HIGH), khi bam xuong mass se la 0 (LOW).
    DDRD = 0b01110000;
    PORTD = 0b00000000;
    DDRB = 0x00;
    PORTB = 0x0D;
}

// ham tat tat ca cac led
void leds_off(void) {
    LED_R = 0;
    LED_G = 0;
    LED_B = 0;
}
// Ham chinh
void main(void) {
    io_init();

    while (1) {

        if (BT_1 == 0) {
            leds_off();     // Tat het cac LED khac
            LED_G = 1;      // Bat LED Xanh la (Green)
        }

        else if (BT_2 == 0) {
            leds_off();     // Tat het cac LED khac
            LED_B = 1;      // Bat LED Xanh duong (Blue)
        }

        else if (BT_3 == 0) {
            leds_off();     // Tat het cac LED khac
            LED_R = 1;      // Bat LED Do (Red)
        }

        else {
            leds_off();     // Tat tat ca cac LED
        }
        delay_ms(10);
    }
}