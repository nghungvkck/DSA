// Doc cam bien DHT11, hien thi nhiet do + do am len LCD 16x2

#include <io.h>
#include <alcd.h>
#include <delay.h>

// Chan data DHT11 noi vao PB0
#define PIN_DHT  PINB.0
#define PORT_DHT PORTB.0
#define DDR_DHT  DDRB.0

unsigned char nhietdo, doam;

// Doc DHT11: tra ve 1 neu thanh cong, 0 neu loi
//   *t  = nhiet do (do C)
//   *rh = do am (%)
char dht11(unsigned char *t, unsigned char *rh) {
    unsigned char i, j, sum;
    unsigned char buf[5] = {0, 0, 0, 0, 0};

    // Gui tin hieu START: keo high 60us roi low it nhat 18ms
    DDR_DHT = 1;
    PORT_DHT = 1;
    delay_us(60);
    PORT_DHT = 0;
    delay_ms(20);

    // Tha chan sang input de nhan phan hoi
    PORT_DHT = 1;
    DDR_DHT = 0;
    delay_us(40);

    // Kiem tra phan hoi: 80us low + 80us high
    if (PIN_DHT) return 0;      // van high -> khong co phan hoi
    while (!PIN_DHT);           // cho het xung low
    delay_us(60);
    if (!PIN_DHT) return 0;     // khong high dung -> loi

    // Doc 40 bit (5 byte)
    // Moi bit: 50us low + high (26-28us = 0, 70us = 1)
    // Delay 40us sau suon len de phan biet bit 0/1
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            while (!PIN_DHT);        // cho het xung low
            delay_us(40);            // van high sau 40us -> bit 1
            if (PIN_DHT) {
                buf[i] |= (1 << (7 - j));  // set bit, MSB truoc
                while (PIN_DHT);           // cho het xung high
            }
            else {
                while (PIN_DHT);           // bit 0
            }
        }
    }

    // Kiem tra checksum: byte 5 = tong 4 byte dau (lay 8 bit thap)
    sum = buf[0] + buf[1] + buf[2] + buf[3];
    if (sum != buf[4]) return 0;

    // buf[0] = do am nguyen, buf[2] = nhiet do nguyen
    *rh = buf[0];
    *t  = buf[2];
    return 1;
}

void main(void) {
    char dht;

    DDRD = 0x80;
    PORTD = 0x80;   // bat den nen LCD

    lcd_init(16);
    lcd_clear();

    while (1) {
        dht = dht11(&nhietdo, &doam);   // doc cam bien

        // Dong 1: "TEMP: XX*"
        lcd_gotoxy(0, 0);
        lcd_puts("TEMP: ");
        lcd_putchar(nhietdo / 10 + '0');
        lcd_putchar(nhietdo % 10 + '0');
        lcd_putchar('*');
        lcd_puts("  ");

        // Dong 2: "HUM : XX%"
        lcd_gotoxy(0, 1);
        lcd_puts("HUM : ");
        lcd_putchar(doam / 10 + '0');
        lcd_putchar(doam % 10 + '0');
        lcd_putchar('%');
        lcd_puts("  ");

        delay_ms(1000);   // DHT11 can >=1s giua 2 lan doc
    }
}